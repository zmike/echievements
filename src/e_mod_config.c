#include "e_mod_main.h"


static void
_free_data(E_Config_Dialog *cfd EINA_UNUSED, E_Config_Dialog_Data *cfdata EINA_UNUSED)
{
   mod->cfd = NULL;
}
/*
static void *
_create_data(E_Config_Dialog *cfd EINA_UNUSED)
{
   return NULL;
}

static int
_basic_apply(E_Config_Dialog *cfd EINA_UNUSED, E_Config_Dialog_Data *cfdata EINA_UNUSED)
{
   return 0;
}

static int
_basic_check_changed(E_Config_Dialog *cfd EINA_UNUSED, E_Config_Dialog_Data *cfdata EINA_UNUSED)
{
   return 0;
}
*/
static Evas_Object *
_item_create(Evas *evas, const char *name, const char *description, unsigned int goal, unsigned int counter)
{
   Evas_Object *table, *icon, *label;
   char progress[128];

   table = e_widget_frametable_add(evas, name, 0);

   icon = e_widget_image_add_from_file(evas, PACKAGE_DATA_DIR "/trophy.png",
                                       32, 32);
   e_widget_frametable_object_append(table, icon, 0, 0, 1, 2, 1, 1, 1, 0);
   label = e_widget_label_add(evas, description);
   e_widget_frametable_object_append(table, label, 1, 0, 3, 1, 1, 1, 1, 0);

   if (goal < 2)
     {
        if (!counter)
          label = e_widget_label_add(evas, "Not achieved");
        else
          label = e_widget_label_add(evas, "Achieved");
     }
   else
     {
        snprintf(progress, sizeof(progress), "Progress: %u/%u", counter, goal);
        label = e_widget_label_add(evas, progress);
     }
   e_widget_frametable_object_append(table, label, 1, 1, 3, 1, 1, 1, 1, 0);

   return table;
}

static Evas_Object *
_basic_create(E_Config_Dialog *cfd EINA_UNUSED, Evas *evas, E_Config_Dialog_Data *cfdata EINA_UNUSED)
{
   Evas_Object *toolbook, *list, *item, *sf;
   Echievement *ec;
   int mw, mh;

   toolbook = e_widget_toolbook_add(evas, 48 * e_scale, 48 * e_scale);

   /* FIXME it should be scrollable and all initial items should
      be visible */
   list = e_widget_list_add(evas, 1, 0);
   e_util_size_debug_set(list, 1);
   EINA_INLIST_FOREACH(EINA_INLIST_GET(mod->trophies_list), ec)
     {
        unsigned int goal, counter;
        if (!etrophy_trophy_earned_get(ec->trophy)) continue;
        etrophy_trophy_goal_get(ec->trophy, &goal, &counter);
        item = _item_create(evas, etrophy_trophy_name_get(ec->trophy),
                            etrophy_trophy_description_get(ec->trophy),
                            goal, counter);
        e_widget_list_object_append(list, item, 1, 1, 0.5);
     }
   e_widget_size_min_get(list, &mw, &mh);

   if (mw < 320) mw = 320;
   if (mh < 220) mh = 220;

   evas_object_resize(list, mw, mh);
   sf = e_widget_scrollframe_simple_add(evas, list);
   e_widget_size_min_set(sf, 320, 220);
   e_widget_toolbook_page_append(toolbook, NULL, "My Trophies",
                                 sf, 1, 1, 1, 1, 0.5, 0.0);

   list = e_widget_list_add(evas, 1, 0);

   EINA_INLIST_FOREACH(EINA_INLIST_GET(mod->trophies_list), ec)
     {
        unsigned int goal, counter;
        if ((!etrophy_trophy_earned_get(ec->trophy)) && (!etrophy_trophy_visibility_get(ec->trophy))) continue;
        etrophy_trophy_goal_get(ec->trophy, &goal, &counter);
        item = _item_create(evas, etrophy_trophy_name_get(ec->trophy),
                            etrophy_trophy_description_get(ec->trophy),
                            goal, counter);
        e_widget_list_object_append(list, item, 1, 1, 0.5);
     }
   e_widget_size_min_get(list, &mw, &mh);

   if (mw < 320) mw = 320;
   if (mh < 220) mh = 220;

   evas_object_resize(list, mw, mh);
   sf = e_widget_scrollframe_simple_add(evas, list);
   e_widget_size_min_set(sf, 320, 220);
   e_widget_toolbook_page_append(toolbook, NULL, "All Trophies",
                                 sf, 1, 1, 1, 1, 0.5, 0.0);

   e_widget_toolbook_page_show(toolbook, 0);
   return toolbook;
}

E_Config_Dialog *
e_int_config_echievements(E_Container *con, const char *params EINA_UNUSED)
{
   E_Config_Dialog *cfd = NULL;
   E_Config_Dialog_View *v = NULL;
   char buf[4096];

   if (e_config_dialog_find("Echievements", "extensions/echievements"))
     return NULL;

   v = E_NEW(E_Config_Dialog_View, 1);
   v->free_cfdata = _free_data;
   v->basic.create_widgets = _basic_create;
/*
   v->create_cfdata = _create_data;
   v->basic.apply_cfdata = _basic_apply;
   v->basic.check_changed = _basic_check_changed;
*/
   snprintf(buf, sizeof(buf), "%s/e-module-echievements.edj",
            mod->module->dir);

   cfd = e_config_dialog_new(con, D_("Echievements"), "Echievements",
                             "extensions/echievements", buf, 0, v, NULL);

   e_dialog_resizable_set(cfd->dia, 1);
   mod->cfd = cfd;
   return cfd;
}
