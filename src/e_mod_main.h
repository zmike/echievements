#ifndef E_MOD_MAIN_H
#define E_MOD_MAIN_H

#include <e.h>
#include <Etrophy.h>
#include <E_Notify.h>

#ifdef ENABLE_NLS
# include <libintl.h>
# define D_(string) dgettext(PACKAGE, string)
#else
# define bindtextdomain(domain,dir)
# define bind_textdomain_codeset(domain,codeset)
# define D_(string) (string)
#endif

#define MOD_CONFIG_FILE_EPOCH 0x0001
#define MOD_CONFIG_FILE_GENERATION 0x01
#define MOD_CONFIG_FILE_VERSION \
   ((MOD_CONFIG_FILE_EPOCH << 16) | MOD_CONFIG_FILE_GENERATION)


/* See instructions in echievements.x for adding new ones */
typedef enum
{
   ECHIEVEMENT_ID_SHELF_POSITIONS,
   ECHIEVEMENT_ID_NOTHING_ELSE_MATTERS,
   ECHIEVEMENT_ID_LAST
} Echievement_Id;

typedef struct Echievement
{
   EINA_INLIST;
   Echievement_Id id;
   Etrophy_Trophy *trophy;
   Eina_List *handlers;
} Echievement;

typedef void (*Echievement_Init_Cb)(Echievement*);

typedef struct Mod
{
   E_Config_Dialog *cfd;
   E_Module *module;
   Eina_Hash *trophies; // for fast lookups
   Echievement *trophies_list; // head for iterating
   Eina_Inlist *itr; // current iterator position if needed
} Mod;

typedef struct Config
{
   unsigned int config_version;
   Etrophy_Gamescore *gs;
} Config;

extern Mod *mod;
extern Config *ech_config;

E_Config_Dialog *e_int_config_echievements(E_Container *con, const char *params);

void ech_init(void);
void ech_shutdown(void);

#endif
