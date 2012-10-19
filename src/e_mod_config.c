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
_basic_check_changed(E_Config_Dialog *cfd  EINA_UNUSED, E_Config_Dialog_Data *cfdata)
{
   return 0;
}

static Evas_Object *
_basic_create(E_Config_Dialog *cfd  EINA_UNUSED, Evas *evas, E_Config_Dialog_Data *cfdata)
{
   return NULL;
}

E_Config_Dialog *
e_int_config_echievements(E_Container *con, const char *params EINA_UNUSED)
{
   E_Config_Dialog *cfd = NULL;
   E_Config_Dialog_View *v = NULL;
   char buf[4096];

   if (e_config_dialog_find("Echievements", "extensions/echievements")) return NULL;

   v = E_NEW(E_Config_Dialog_View, 1);
   v->create_cfdata = _create_data;
   v->free_cfdata = _free_data;
   v->basic.create_widgets = _basic_create;
   v->basic.apply_cfdata = _basic_apply;
   v->basic.check_changed = _basic_check_changed;

   snprintf(buf, sizeof(buf), "%s/e-module-echievements.edj", mod->module->dir);

   cfd = e_config_dialog_new(con, D_("Echievements Module"), "Echievements", 
                             "extensions/echievements", buf, 0, v, NULL);

   e_dialog_resizable_set(cfd->dia, 1);
   mod->cfd = cfd;
   return cfd;
}
