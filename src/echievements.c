#include "e_mod_main.h"
#include "echievements.x"

#define ECH_INIT(NAME)     void echievement_init_cb_##NAME(Echievement *ec)
#define ECH_EH_NAME(NAME)  echievement_##NAME##_handler_cb
#define ECH_EH(NAME, TYPE) static Eina_Bool ECH_EH_NAME(NAME)(Echievement *ec, int type EINA_UNUSED, TYPE *ev)
#define ECH_BH_NAME(NAME, TYPE)  echievement_##NAME##_border_hook##TYPE
#define ECH_BH(NAME, TYPE) static void ECH_BH_NAME(NAME, TYPE)(Echievement *ec, E_Border *bd)
#define ECH_BH_ADD(NAME, TYPE) \
   ec->handlers = eina_list_append(ec->handlers, \
     e_border_hook_add(E_BORDER_HOOK_##TYPE, (Ecore_End_Cb)ECH_BH_NAME(WINDOW_MOVER, TYPE), ec))
#define ECH_MH_NAME(NAME) _ech_##NAME##_mouse_hook
#define ECH_MH(NAME) static void ECH_MH_NAME(NAME)(Echievement *ec)
#define ECH_MH_ADD(NAME) \
 do { \
    ec->mouse_hook = (Ecore_Cb)ECH_MH_NAME(NAME); \
    if (!etrophy_trophy_earned_get(ec->trophy)) \
      mod->mouse.hooks = eina_list_append(mod->mouse.hooks, ec); \
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
   _ech_list_add(et);
}

static void
_ech_free(Echievement *ec)
{
   /* trophy does NOT get freed here!!! */
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
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
   etrophy_trophy_counter_set(et, Echievement_Goals[id]);
   INF("TROPHY EARNED: %s - %s",
       etrophy_trophy_name_get(et),
       etrophy_trophy_description_get(et));
   _ech_notify(etrophy_trophy_name_get(et),
               etrophy_trophy_description_get(et));
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
   return ECORE_CALLBACK_CANCEL;
}

ECH_EH(CAVE_DWELLER, void EINA_UNUSED)
{
   if (e_backlight_level_get(e_util_zone_current_get(e_manager_current_get())) >= 1.)
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   return ECORE_CALLBACK_CANCEL;
}

ECH_EH(WINDOW_ENTHUSIAST, void EINA_UNUSED)
{
   /* ignore all windows added before modules are loaded:
    * these are previously-placed windows
    */
   if (!mod->module_init_end) return ECORE_CALLBACK_RENEW;

   etrophy_trophy_counter_increment(ec->trophy, 1);
   if (!etrophy_trophy_earned_get(ec->trophy))
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   return ECORE_CALLBACK_CANCEL;
}

ECH_EH(AFRAID_OF_THE_DARK, void EINA_UNUSED)
{
   if (e_backlight_level_get(e_util_zone_current_get(e_manager_current_get())) <= 99.)
     return ECORE_CALLBACK_RENEW;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   return ECORE_CALLBACK_CANCEL;
}

ECH_EH(SHELF_POSITIONS, E_Event_Shelf EINA_UNUSED)
{
   if (eina_list_count(e_shelf_list()) != Echievement_Goals[ec->id])
     {
        etrophy_trophy_counter_set(ec->trophy,
                                   eina_list_count(e_shelf_list()));
        return ECORE_CALLBACK_RENEW;
     }
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, ecore_event_handler_del);
   return ECORE_CALLBACK_CANCEL;
}

///////////////////////////////////////////////////////////////////////////////////

/* Echievement border hook callbacks */

ECH_BH(WINDOW_MOVER, MOVE_END)
{
   etrophy_trophy_counter_increment(ec->trophy, 1);
   if (!etrophy_trophy_earned_get(ec->trophy)) return;
   _ech_hook(ec->id, ec);
   E_FREE_LIST(ec->handlers, e_border_hook_del);
   (void)bd;
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
   etrophy_trophy_counter_increment(ec->trophy, abs(mod->mouse.dx) + abs(mod->mouse.dy));
   if (!etrophy_trophy_earned_get(ec->trophy)) return;
   _ech_hook(ec->id, ec);
   mod->mouse.hooks = eina_list_remove(mod->mouse.hooks, ec);
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

ECH_INIT(WINDOW_STALKER)
{
   /* only count windows opened while e is running to prevent cheating
    * reuse same handler
    */
   E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_BORDER_ADD, ECH_EH_NAME(WINDOW_ENTHUSIAST), ec);
}

ECH_INIT(WINDOW_LOVER)
{
   /* only count windows opened while e is running to prevent cheating
    * reuse same handler
    */
   E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_BORDER_ADD, ECH_EH_NAME(WINDOW_ENTHUSIAST), ec);
}

ECH_INIT(WINDOW_ENTHUSIAST)
{
   /* only count windows opened while e is running to prevent cheating */
   ECH_BH_ADD(WINDOW_MOVER, MOVE_END);
}

ECH_INIT(WINDOW_OCD)
{
   /* only count windows moved while e is running to prevent cheating */
   ECH_BH_ADD(WINDOW_MOVER, MOVE_END);
}

ECH_INIT(WINDOW_MOVER)
{
   /* only count windows moved while e is running to prevent cheating */
   ECH_BH_ADD(WINDOW_MOVER, MOVE_END);
}

ECH_INIT(SHELF_POSITIONS)
{
   if (eina_list_count(e_shelf_list()) == Echievement_Goals[ec->id])
     /* number of shelves equals goal, grant trophy and return */
     _ech_hook(ec->id, ec);
   else
     E_LIST_HANDLER_APPEND(ec->handlers, E_EVENT_SHELF_ADD, ECH_EH_NAME(SHELF_POSITIONS), ec);
}

ECH_INIT(MOUSE_RUNNER)
{
   ECH_MH_ADD(MOUSE_RUNNER);
}

ECH_INIT(MOUSE_MARATHONER)
{
   ECH_MH_ADD(MOUSE_RUNNER);
}

ECH_INIT(MOUSE_HERO)
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
        E_LIST_FOREACH(etrophy_gamescore_trophies_list_get(ech_config->gs), _ech_list_add);
        _ech_idler = ecore_idler_add(_ech_init_check_idler, NULL);
     }
   else
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

