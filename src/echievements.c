#include "e_mod_main.h"
#include "echievements.x"

#define ECH_INIT(NAME)     EINTERN void echievement_init_cb_##NAME(Echievement *ec)
#define ECH_EH_NAME(NAME)  echievement_##NAME##_handler_cb
#define ECH_EH(NAME, TYPE) static Eina_Bool ECH_EH_NAME(NAME)(Echievement *ec, int type, TYPE *ev)
#define ECH_BH_NAME(NAME, TYPE)  echievement_##NAME##_border_hook_##TYPE
#define ECH_BH(NAME, TYPE) static void ECH_BH_NAME(NAME, TYPE)(Echievement *ec, E_Border *bd)
#define ECH_BH_ADD(NAME, TYPE) \
   do { \
      if (etrophy_trophy_earned_get(ec->trophy)) break; \
      ec->bh_handlers = 1; \
      ec->handlers = eina_list_append(ec->handlers, \
        e_border_hook_add(E_BORDER_HOOK_##TYPE, (Ecore_End_Cb)ECH_BH_NAME(NAME, TYPE), ec)); \
   } while (0)
#define ECH_MH_NAME(NAME) _ech_##NAME##_mouse_hook
#define ECH_MH(NAME) static void ECH_MH_NAME(NAME)(Echievement *ec)
#define ECH_MH_ADD(NAME) \
   do { \
      ec->mouse_hook = (Ecore_Cb)ECH_MH_NAME(NAME); \
      if (!etrophy_trophy_earned_get(ec->trophy)) \
        mod->mouse.hooks = eina_list_append(mod->mouse.hooks, ec); \
   } while (0)
#define ECH_MH_DEL \
   do { \
     ec->mouse_hook = NULL; \
     mod->mouse.hooks = eina_list_remove(mod->mouse.hooks, ec); \
   } while (0)

static Ecore_Idler *_ech_idler = NULL;

/* helper for init */
static void
_ech_list_add(Etrophy_Trophy *et)
{
   Echievement *ec;

   ec = E_NEW(Echievement, 1);
   ec->trophy = et;
   ec->id = eina_hash_population(mod->trophies);
   eina_hash_add(mod->trophies, &ec->id, ec);
   mod->trophies_list = (Echievement *)eina_inlist_append(EINA_INLIST_GET(mod->trophies_list), EINA_INLIST_GET(ec));
}

static void
_ech_add(Echievement_Id id)
{
   Etrophy_Trophy *et;

   et = etrophy_trophy_new(Echievement_Strings[id], Echievement_Descs[id],
                           Echievement_Hide_States[id], Echievement_Goals[id]);
   etrophy_gamescore_trophy_add(ech_config->gs, et);
   _ech_list_add(et);
}

static void
_ech_free(Echievement *ec)
{
   /* trophy does NOT get freed here!!! */
   if (ec->bh_handlers)
     E_FREE_LIST(ec->handlers, e_border_hook_del);
   else
     E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   if (ec->mouse_hook) mod->mouse.hooks = eina_list_remove(mod->mouse.hooks, ec);
   if (ec->dialog.icon)
     evas_object_event_callback_del_full(ec->dialog.icon, EVAS_CALLBACK_DEL, mod->obj_del_cb, ec);
   if (ec->dialog.label)
     evas_object_event_callback_del_full(ec->dialog.label, EVAS_CALLBACK_DEL, mod->obj_del_cb, ec);
   free(ec);
}

/* returns trophy only if it has not yet been earned */
static Etrophy_Trophy *
_ech_lookup(Echievement_Id id)
{
   Echievement *ec;

   ec = eina_hash_find(mod->trophies, &id);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ec, NULL);
   if (!etrophy_trophy_earned_get(ec->trophy)) return ec->trophy;
   INF("TROPHY PREVIOUSLY EARNED: %s - %s",
       etrophy_trophy_name_get(ec->trophy),
       etrophy_trophy_description_get(ec->trophy));
   return NULL;
}

static void
_ech_notify(const char *name, const char *description)
{
   E_Notification *n;
   char summary[128];

   snprintf(summary, sizeof(summary), "Echievement: %s", name);
   n = e_notification_full_new("echievements", 0, PACKAGE_DATA_DIR"/trophy.png",
                               summary, description, -1);
   e_notification_hint_urgency_set(n, E_NOTIFICATION_URGENCY_NORMAL);
   e_notification_send(n, NULL, NULL);
   e_notification_unref(n);
}

static void
_ech_trophy_counter_increment(Echievement *ec, unsigned int value)
{
   if (etrophy_trophy_earned_get(ec->trophy)) return;
   etrophy_trophy_counter_increment(ec->trophy, value);
   ech_cfg_ech_update(ec);
}

static void
_ech_trophy_counter_set(Echievement *ec, unsigned int value)
{
   unsigned int count;

   if (etrophy_trophy_earned_get(ec->trophy)) return;
   etrophy_trophy_goal_get(ec->trophy, NULL, &count);
   if (value == count) return;
   etrophy_trophy_counter_set(ec->trophy, value);
   ech_cfg_ech_update(ec);
}

/* call whenever new echievement is earned, ec may be NULL */
static void
_ech_hook(Echievement_Id id, Echievement *ec)
{
   Etrophy_Trophy *et;

   if (ec)
     et = ec->trophy;
   else
     et = _ech_lookup(id);
   if (!et) return;
   if (ec)
     _ech_trophy_counter_set(ec, Echievement_Goals[id]);
   else //FIXME?
     etrophy_trophy_counter_set(et, Echievement_Goals[id]);
   INF("TROPHY EARNED: %s - %s",
       etrophy_trophy_name_get(et),
       etrophy_trophy_description_get(et));
   _ech_notify(etrophy_trophy_name_get(et),
               etrophy_trophy_description_get(et));
   if (ec) ech_cfg_ech_add(ec);
}

static Eina_Bool
_ech_init_check_idler(void *d EINA_UNUSED)
{
   Echievement *ec;
   unsigned int count = 0;
   Eina_Inlist *l;

   l = mod->itr ? : EINA_INLIST_GET(mod->trophies_list);
   EINA_INLIST_FOREACH(l, ec)
     {
        if (count > 20)
          {
             mod->itr = EINA_INLIST_GET(ec);
             return EINA_TRUE;
          }
        if (etrophy_trophy_earned_get(ec->trophy)) continue;
        if (!Echievement_Callbacks[ec->id]) continue;
        Echievement_Callbacks[ec->id](ec);
        count++;
     }
   _ech_idler = ecore_idler_del(_ech_idler);
   return EINA_FALSE;
}

static Eina_Bool
_ech_init_add_idler(void *d EINA_UNUSED)
{
   Echievement_Id id = 0;
   unsigned int count = 0;

   if (mod->trophies_list)
     {
        Echievement *ec;

        ec = EINA_INLIST_CONTAINER_GET(EINA_INLIST_GET(mod->trophies_list)->last, Echievement);
        id = ec->id + 1;
     }
   for (; id < ECH(LAST); count++, id++)
     {
        if (count > 20) return EINA_TRUE;
        _ech_add(id);
     }
   ecore_idler_del(_ech_idler);
   _ech_idler = ecore_idler_add(_ech_init_check_idler, NULL);
   return EINA_FALSE;
}

static Eina_Bool
NOT_SO_INCOGNITO_helper(const char *str)
{
   unsigned int x;

   if ((!str) || (!str[0])) return EINA_FALSE;
   for (x = 0; Echievement_NOT_SO_INCOGNITO_Strings[x]; x++)
     if (!strncmp(str, Echievement_NOT_SO_INCOGNITO_Strings[x], sizeof(Echievement_NOT_SO_INCOGNITO_Strings[x] - 1)))
       return EINA_TRUE;
   return EINA_FALSE;
}

///////////////////////////////////////////////////////////////////////////////////

/* Echievement event handler callbacks:
 *
 * appended in related ECH_INIT, data param is the echievement
 *
 * check conditions, increment/set counter, delete handlers if trophy acquired
 */

ECH_EH(NOTHING_ELSE_MATTERS, E_Event_Shelf EINA_UNUSED)
{
   if (eina_list_count(e_shelf_list()))
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(CAVE_DWELLER, void EINA_UNUSED)
{
   if (e_backlight_level_get(e_util_zone_current_get(e_manager_current_get())) >= 1.)
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(WINDOW_ENTHUSIAST, void EINA_UNUSED)
{
   /* ignore all windows added before modules are loaded:
    * these are previously-placed windows
    */
   if (!mod->module_init_end) return ECORE_CALLBACK_RENEW;

   _ech_trophy_counter_increment(ec,  1);
   if (!etrophy_trophy_earned_get(ec->trophy))
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(AFRAID_OF_THE_DARK, void EINA_UNUSED)
{
   if (e_backlight_level_get(e_util_zone_current_get(e_manager_current_get())) <= 99.)
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(DUALIST, E_Event_Zone_Add)
{
   _ech_trophy_counter_set(ec, eina_list_count(ev->zone->container->zones));
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(SHELF_POSITIONS, E_Event_Shelf EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, eina_list_count(e_shelf_list()));
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(GOING_HD, E_Event_Zone_Add EINA_UNUSED)
{
   E_Screen *es;
   const Eina_List *l;

   EINA_LIST_FOREACH(e_xinerama_screens_get(), l, es)
     _ech_trophy_counter_set(ec, es->w * es->h);
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(REAL_ESTATE_MOGUL, E_Event_Zone_Add EINA_UNUSED)
{
   E_Screen *es;
   const Eina_List *l;
   unsigned int geom = 0;

   EINA_LIST_FOREACH(e_xinerama_screens_get(), l, es)
     geom += (es->w * es->h);
   _ech_trophy_counter_set(ec, geom);
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(MAXIMUM_DEFINITION, E_Event_Zone_Add EINA_UNUSED)
{
   E_Screen *es;
   const Eina_List *l;
   unsigned int geom = 0;

   EINA_LIST_FOREACH(e_xinerama_screens_get(), l, es)
     geom += ((es->w * es->h) >= (1920 * 1080));
   _ech_trophy_counter_set(ec, geom);
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(EDGY, void EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_edge());
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(SLEEPER, void EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_acpi());
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(SIGNALLER, void EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_signal());
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(WHEELY, void EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_wheel());
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(CLICKER, void EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_mouse());
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(KEYBOARD_USER, void EINA_UNUSED)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_keys());
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(GADGETEER, E_Event_Gadcon_Client_Add EINA_UNUSED)
{
   E_Config_Gadcon *cf_gc;
   Eina_List *l;
   unsigned int gadgets = 0;

   /* not gonna fuck with trying to keep an accurate count here,
    * gadman/gadcon is too shitty
    */
   EINA_LIST_FOREACH(e_config->gadcons, l, cf_gc)
     if (!e_util_strcmp(cf_gc->name, "gadman"))
       gadgets += eina_list_count(cf_gc->clients);
   _ech_trophy_counter_set(ec, gadgets);
   if (!etrophy_trophy_earned_get(ec->trophy)) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(PHYSICIST, E_Event_Module_Update)
{
   if ((!ev->enabled) || e_util_strcmp(ev->name, "Physics")) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(LIFE_ON_THE_EDGE, E_Event_Module_Update EINA_UNUSED)
{
   const char *env;

   env = getenv("E17_TAINTED");
   if (e_util_strcmp(env, "YES")) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(QUICKDRAW, E_Event_Module_Update)
{
   if ((!ev->enabled) || e_util_strcmp(ev->name, "Quickaccess")) return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   (void)type;
   return ECORE_CALLBACK_RENEW;
}

ECH_EH(OPAQUE, E_Event_Module_Update)
{
   if (type == E_EVENT_MODULE_INIT_END)
     {
        if (e_module_find("Composite")) return ECORE_CALLBACK_RENEW;
     }
   else
     {
        if ((ev->enabled) || e_util_strcmp(ev->name, "Composite"))
          return ECORE_CALLBACK_RENEW;
     }
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   return ECORE_CALLBACK_RENEW;
}

///////////////////////////////////////////////////////////////////////////////////

/* Echievement mouse hook callbacks:
 *
 * called every time the mouse moves
 *
 * mod->mouse.dx/dy are change since last call
 */

ECH_MH(MOUSE_RUNNER)
{
   _ech_trophy_counter_increment(ec,  abs(mod->mouse.dx) + abs(mod->mouse.dy));
   if (!etrophy_trophy_earned_get(ec->trophy)) return;
   _ech_hook(ec->id, ec);
   ECH_MH_DEL;
}

ECH_MH(WINDOW_HAULER)
{
   _ech_trophy_counter_increment(ec,  abs(mod->mouse.dx) + abs(mod->mouse.dy));
   if (!etrophy_trophy_earned_get(ec->trophy)) return;
   _ech_hook(ec->id, ec);
   ECH_MH_DEL;
   E_FREE_LIST(ec->handlers, e_border_hook_del);
}

///////////////////////////////////////////////////////////////////////////////////

/* Echievement border hook callbacks */

ECH_BH(WINDOW_HAULER, MOVE_BEGIN)
{
   ECH_MH_ADD(WINDOW_HAULER);
   (void)bd;
}

ECH_BH(TERMINOLOGIST, EVAL_PRE_POST_FETCH)
{
   if (e_util_strcmp(bd->client.icccm.class, "terminology")) return;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, e_border_hook_del);
}

ECH_BH(NOT_SO_INCOGNITO, EVAL_PRE_POST_FETCH)
{
   if ((!NOT_SO_INCOGNITO_helper(bd->client.icccm.title)) && (!NOT_SO_INCOGNITO_helper(bd->client.netwm.name)))
     return;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, e_border_hook_del);
}

ECH_BH(WINDOW_HAULER, MOVE_END)
{
   ECH_MH_DEL;
   (void)bd;
}

ECH_BH(WINDOW_MOVER, MOVE_END)
{
   _ech_trophy_counter_increment(ec,  1);
   if (!etrophy_trophy_earned_get(ec->trophy)) return;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, e_border_hook_del);
   (void)bd;
}

///////////////////////////////////////////////////////////////////////////////////

/* Echievement init callbacks:
 *
 * called every time the module loads upon loading a trophy if the trophy has not been acquired
 *
 * ** check trophy status
 *  a. if goal met, hook echievement
 *  b. else add handlers
 */

ECH_INIT(NOTHING_ELSE_MATTERS)
{
   if (!eina_list_count(e_shelf_list()))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_SHELF_DEL, ECH_EH_NAME(NOTHING_ELSE_MATTERS), ec);
}

ECH_INIT(AFRAID_OF_THE_DARK)
{
   if (e_backlight_level_get(e_util_zone_current_get(e_manager_current_get())) > 99.)
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_BACKLIGHT_CHANGE, ECH_EH_NAME(AFRAID_OF_THE_DARK), ec);
}

ECH_INIT(CAVE_DWELLER)
{
   if (e_backlight_level_get(e_util_zone_current_get(e_manager_current_get())) < 1.)
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_BACKLIGHT_CHANGE, ECH_EH_NAME(CAVE_DWELLER), ec);
}

ECH_INIT(WINDOW_ENTHUSIAST)
{
   /* only count windows opened while e is running to prevent cheating
    * reuse same handler
    */
   E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_BORDER_ADD, ECH_EH_NAME(WINDOW_ENTHUSIAST), ec);
}

ECH_INIT(WINDOW_MOVER)
{
   /* only count windows moved while e is running to prevent cheating */
   ECH_BH_ADD(WINDOW_MOVER, MOVE_END);
}

ECH_INIT(WINDOW_HAULER)
{
   ECH_BH_ADD(WINDOW_HAULER, MOVE_BEGIN);
   ECH_BH_ADD(WINDOW_HAULER, MOVE_END);
}

ECH_INIT(EDGY)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_edge());
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MANAGER_KEYS_GRAB, ECH_EH_NAME(EDGY), ec);
}

ECH_INIT(SLEEPER)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_acpi());
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MANAGER_KEYS_GRAB, ECH_EH_NAME(SLEEPER), ec);
}

ECH_INIT(SIGNALLER)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_signal());
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MANAGER_KEYS_GRAB, ECH_EH_NAME(SIGNALLER), ec);
}

ECH_INIT(WHEELY)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_wheel());
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MANAGER_KEYS_GRAB, ECH_EH_NAME(WHEELY), ec);
}

ECH_INIT(CLICKER)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_mouse());
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MANAGER_KEYS_GRAB, ECH_EH_NAME(CLICKER), ec);
}

ECH_INIT(KEYBOARD_USER)
{
   _ech_trophy_counter_set(ec, ech_bindings_check_keys());
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MANAGER_KEYS_GRAB, ECH_EH_NAME(KEYBOARD_USER), ec);
}

ECH_INIT(TERMINOLOGIST)
{
   Eina_List *l;
   E_Border *bd;

   EINA_LIST_FOREACH(e_border_client_list(), l, bd)
     if (!e_util_strcmp(bd->client.icccm.class, "terminology"))
       _ech_hook(ec->id, ec);
   if (!etrophy_trophy_earned_get(ec->trophy))
     ECH_BH_ADD(TERMINOLOGIST, EVAL_PRE_POST_FETCH);
}

ECH_INIT(PHYSICIST)
{
   if (e_module_find("Physics"))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MODULE_UPDATE, ECH_EH_NAME(PHYSICIST), ec);
}

ECH_INIT(QUICKDRAW)
{
   if (e_module_find("Quickaccess"))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MODULE_UPDATE, ECH_EH_NAME(QUICKDRAW), ec);
}

ECH_INIT(LIFE_ON_THE_EDGE)
{
   const char *env;

   env = getenv("E17_TAINTED");
   if (!e_util_strcmp(env, "YES"))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MODULE_UPDATE, ECH_EH_NAME(LIFE_ON_THE_EDGE), ec);
}

ECH_INIT(OPAQUE)
{
   E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MODULE_UPDATE, ECH_EH_NAME(OPAQUE), ec);
   E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_MODULE_INIT_END, ECH_EH_NAME(OPAQUE), ec);
}

ECH_INIT(DUALIST)
{
   E_Container *con;

   con = e_util_container_current_get();
   if (con)
     _ech_trophy_counter_set(ec, eina_list_count(con->zones));
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_ZONE_ADD, ECH_EH_NAME(DUALIST), ec);
}

ECH_INIT(SHELF_POSITIONS)
{
   _ech_trophy_counter_set(ec, eina_list_count(e_shelf_list()));
   if (etrophy_trophy_earned_get(ec->trophy))
     /* number of shelves equals goal, grant trophy and return */
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_SHELF_ADD, ECH_EH_NAME(SHELF_POSITIONS), ec);
}

ECH_INIT(GOING_HD)
{
   E_Screen *es;
   const Eina_List *l;

   EINA_LIST_FOREACH(e_xinerama_screens_get(), l, es)
     _ech_trophy_counter_set(ec, es->w * es->h);
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_ZONE_ADD, ECH_EH_NAME(GOING_HD), ec);
}

ECH_INIT(REAL_ESTATE_MOGUL)
{
   E_Screen *es;
   const Eina_List *l;
   unsigned int geom = 0;

   EINA_LIST_FOREACH(e_xinerama_screens_get(), l, es)
     geom += (es->w * es->h);
   _ech_trophy_counter_set(ec, geom);
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_ZONE_ADD, ECH_EH_NAME(REAL_ESTATE_MOGUL), ec);
}

ECH_INIT(MAXIMUM_DEFINITION)
{
   E_Screen *es;
   const Eina_List *l;
   unsigned int geom = 0;

   EINA_LIST_FOREACH(e_xinerama_screens_get(), l, es)
     geom += ((es->w * es->h) >= (1920 * 1080));
   _ech_trophy_counter_set(ec, geom);
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_ZONE_ADD, ECH_EH_NAME(MAXIMUM_DEFINITION), ec);
}

ECH_INIT(NOT_SO_INCOGNITO)
{
   Eina_List *l;
   E_Border *bd;

   EINA_LIST_FOREACH(e_border_client_list(), l, bd)
     {
        if (NOT_SO_INCOGNITO_helper(bd->client.icccm.title) || NOT_SO_INCOGNITO_helper(bd->client.netwm.name))
          {
             _ech_hook(ec->id, ec);
             return;
          }
     }
   ECH_BH_ADD(NOT_SO_INCOGNITO, EVAL_PRE_POST_FETCH);
}

ECH_INIT(BILINGUAL)
{
   Eina_List *l;

   // this is very expensive, so don't do it if we don't have to
   if (ec->id == ECH(POLYGLOT))
     if (!etrophy_trophy_earned_get(_ech_lookup(ECH(BILINGUAL)))) return;
   l = ech_language_enumerate();
   _ech_trophy_counter_set(ec, eina_list_count(l));
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   eina_list_free(l);
}

ECH_INIT(GADGETEER)
{
   E_Config_Gadcon *cf_gc;
   Eina_List *l;
   unsigned int gadgets = 0;

   EINA_LIST_FOREACH(e_config->gadcons, l, cf_gc)
     if (!e_util_strcmp(cf_gc->name, "gadman"))
       gadgets += eina_list_count(cf_gc->clients);
   _ech_trophy_counter_set(ec, gadgets);
   if (etrophy_trophy_earned_get(ec->trophy))
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_GADCON_CLIENT_ADD, ECH_EH_NAME(GADGETEER), ec);
}

ECH_INIT(MOUSE_RUNNER)
{
   ECH_MH_ADD(MOUSE_RUNNER);
}

void
ech_init(void)
{
   /* FIXME: delay this */
   mod->trophies = eina_hash_int32_new((Eina_Free_Cb)_ech_free);
   if (ech_config->gs)
     {
        const Eina_List *l;

        l = etrophy_gamescore_trophies_list_get(ech_config->gs);
        if (l)
          {
             E_LIST_FOREACH(l, _ech_list_add);
             _ech_idler = ecore_idler_add(_ech_init_check_idler, NULL);
             return;
          }
     }
   _ech_idler = ecore_idler_add(_ech_init_add_idler, NULL);
}

void
ech_shutdown(void)
{
   eina_hash_free(mod->trophies);
   mod->trophies = NULL;
   mod->trophies_list = NULL;
   mod->itr = NULL;
   if (_ech_idler) _ech_idler = ecore_idler_del(_ech_idler);
}

