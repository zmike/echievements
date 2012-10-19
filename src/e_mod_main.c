#include "e_mod_main.h"

EAPI E_Module_Api e_modapi = {E_MODULE_API_VERSION, "Echievements"};
static E_Config_DD *conf_edd = NULL;
Mod *mod = NULL;
Config *ech_config = NULL;

static void
_e_mod_ech_config_free(void)
{
   etrophy_gamescore_free(ech_config->gs);
   E_FREE(ech_config);
}

static void
_e_mod_ech_config_load(void)
{
#undef T
#undef D
   conf_edd = E_CONFIG_DD_NEW("Config", Config);
   #define T Config
   #define D conf_edd
   E_CONFIG_VAL(D, T, config_version, UINT);
   E_CONFIG_SUB(D, T, gs, etrophy_gamescore_edd_get());

   ech_config = e_config_domain_load("module.echievements", conf_edd);
   if (ech_config)
     {
        if (!e_util_module_config_check("Echievements", ech_config->config_version, MOD_CONFIG_FILE_VERSION))
          _e_mod_ech_config_free();
     }

   if (!ech_config)
     {
        ech_config = E_NEW(Config, 1);
        ech_config->config_version = (MOD_CONFIG_FILE_EPOCH << 16);
     }
}

EAPI void *
e_modapi_init(E_Module *m)
{
   char buf[4096];

   snprintf(buf, sizeof(buf), "%s/locale", e_module_dir_get(m));
   bindtextdomain(PACKAGE, buf);
   bind_textdomain_codeset(PACKAGE, "UTF-8");

   snprintf(buf, sizeof(buf), "%s/e-module-echievements.edj", m->dir);

   e_configure_registry_category_add("extensions", 80, D_("Extensions"),
                                     NULL, "preferences-extensions");
   e_configure_registry_item_add("extensions/echievements", 110, D_("Echievements"),
                                 NULL, buf, e_int_config_echievements);

   etrophy_init();

   mod = E_NEW(Mod, 1);
   mod->module = m;

   _e_mod_ech_config_load();
   ech_init();

   return m;
}

EAPI int
e_modapi_shutdown(E_Module *m EINA_UNUSED)
{
   e_configure_registry_item_del("extensions/echievements");

   e_configure_registry_category_del("extensions");

   if (mod->cfd) e_object_del(E_OBJECT(mod->cfd));
   ech_shutdown();
   e_config_domain_save("module.echievements", conf_edd, ech_config);
   _e_mod_ech_config_free();
   E_CONFIG_DD_FREE(conf_edd);
   E_FREE(mod);
   etrophy_shutdown();
   return 1;
}

EAPI int
e_modapi_save(E_Module *m EINA_UNUSED)
{
   e_config_domain_save("module.echievements", conf_edd, ech_config);
   return 1;
}

