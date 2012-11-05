#include "e_mod_main.h"

static void *
_create_data(E_Config_Dialog *cfd)
{
   return NULL;
}

static void
_free_data(E_Config_Dialog *cfd, E_Config_Dialog_Data *cfdata)
{
}

static int
_basic_apply(E_Config_Dialog *cfd EINA_UNUSED, E_Config_Dialog_Data *cfdata)
{
   return 0;
}

static int
_basic_check_changed(E_Config_Dialog *cfd EINA_UNUSED, E_Config_Dialog_Data *cfdata)
{
   return 0;
}

static Evas_Object *
_item_create(Evas *evas, const char *name, const char *description, int goal, int counter)
{
   Evas_Object *framelist, *table, *icon, *label;
   char progress[128];

   framelist = e_widget_framelist_add(evas, name, 0);

   table = e_widget_table_add(evas, 1);
   icon = e_widget_image_add_from_file(evas, PACKAGE_DATA_DIR"/trophy.png",
                                       32, 32);
   e_widget_table_object_append(table, icon, 0, 0, 1, 2, 1, 1, 1, 0);
   label = e_widget_label_add(evas, description);
   e_widget_table_object_append(table, label, 1, 0, 3, 1, 1, 1, 1, 0);

   if (goal < 2)
     {
        if (!counter)
          label = e_widget_label_add(evas, "Not achieved");
        else
          label = e_widget_label_add(evas, "Achieved");
     }
   else
     {
        snprintf(progress, sizeof(progress), "Progress: %i/%i", counter, goal);
        label = e_widget_label_add(evas, progress);
     }
   e_widget_table_object_append(table, label, 1, 1, 3, 1, 1, 1, 1, 0);

   e_widget_framelist_object_append(framelist, table);
   return framelist;
}

static Evas_Object *
_basic_create(E_Config_Dialog *cfd EINA_UNUSED, Evas *evas, E_Config_Dialog_Data *cfdata)
{
   Evas_Object *toolbook, *list, *item, *sf;
   Echievement *ec;

   toolbook = e_widget_toolbook_add(evas, 48 * e_scale, 48 * e_scale);

   /* FIXME it should be scrollable and all initial items should
      be visible */
   list = e_widget_list_add(evas, 0, 0);
   sf = e_widget_scrollframe_simple_add(evas, list);
   e_widget_size_min_set(sf, 320, 220);
   e_widget_toolbook_page_append(toolbook, NULL, "My Trophies",
                                 sf, 0, 1, 1, 1, 0.0, 0.0);

   EINA_INLIST_FOREACH(EINA_INLIST_GET(mod->trophies_list), ec)
     {
        int goal, counter;
        etrophy_trophy_goal_get(ec->trophy, &goal, &counter);
        item = _item_create(evas, etrophy_trophy_name_get(ec->trophy),
                            etrophy_trophy_description_get(ec->trophy),
                            goal, counter);
        e_widget_list_object_append(list, item, 1, 1, 0.5);
     }

   list = e_widget_list_add(evas, 0, 0);
   e_widget_toolbook_page_append(toolbook, NULL, "All Trophies",
                                 list, 1, 1, 1, 1, 0.5, 0.0);


   e_widget_toolbook_page_show(toolbook, 0);
   evas_object_show(toolbook);

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
   v->create_cfdata = _create_data;
   v->free_cfdata = _free_data;
   v->basic.create_widgets = _basic_create;
   v->basic.apply_cfdata = _basic_apply;
   v->basic.check_changed = _basic_check_changed;

   snprintf(buf, sizeof(buf), "%s/e-module-echievements.edj",
            mod->module->dir);

   cfd = e_config_dialog_new(con, D_("Echievements"), "Echievements",
                             "extensions/echievements", buf, 0, v, NULL);

   e_dialog_resizable_set(cfd->dia, 1);
   mod->cfd = cfd;
   return cfd;
}
