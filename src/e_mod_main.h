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

#define MOD_CONFIG_FILE_EPOCH 0x0000
#define MOD_CONFIG_FILE_GENERATION 0x01
#define MOD_CONFIG_FILE_VERSION \
   ((MOD_CONFIG_FILE_EPOCH << 16) | MOD_CONFIG_FILE_GENERATION)

#define XSTR(X) #X
#define STRINGIFY(X) XSTR(X)

// need to update these whenever default profile updates I guess
#define DEFAULT_BINDING_COUNT_ACPI 11
#define DEFAULT_BINDING_COUNT_KEY 65
#define DEFAULT_BINDING_COUNT_MOUSE 6
#define DEFAULT_BINDING_COUNT_SIGNAL 28

#define ECH(X) ECHIEVEMENT_ID_##X
/* See instructions in echievements.x for adding new ones */
typedef enum
{
   ECH(SHELF_POSITIONS),
   ECH(NOTHING_ELSE_MATTERS),
   ECH(AFRAID_OF_THE_DARK),
   ECH(CAVE_DWELLER),
   ECH(WINDOW_ENTHUSIAST),
   ECH(WINDOW_LOVER),
   ECH(WINDOW_STALKER),
   ECH(WINDOW_MOVER),
   ECH(WINDOW_OCD),
   ECH(MOUSE_RUNNER),
   ECH(MOUSE_MARATHONER),
   ECH(MOUSE_HERO),
   ECH(MIGHTY_MOUSE),
   ECH(WINDOW_HAULER),
   ECH(WINDOW_SLINGER),
   ECH(WINDOW_SHERPA),
   ECH(PHYSICIST),
   ECH(QUICKDRAW),
   ECH(OPAQUE),
   ECH(KEYBOARD_USER),
   ECH(KEYBOARD_TAPPER),
   ECH(KEYBOARD_ABUSER),
   ECH(KEYBOARD_NINJA),
   ECH(CLICKER),
   ECH(SUPER_CLICKER),
   ECH(SUPER_DUPER_CLICKER),
   ECH(CLICK_MANIAC),
   ECH(SLEEPER),
   ECH(HALTER),
   ECH(EDGY),
   ECH(EDGIER),
   ECH(EDGIEST),
   ECH(EDGAR),
   ECH(WHEELY),
   ECH(ROLLIN),
   ECH(ROLY_POLY),
   ECH(SIGNALLER),
   ECH(SIGNAL_CONTROLLER),
   ECH(SIGNAL_MASTER),
   ECH(SIGURD),
   ECH(NOT_SO_INCOGNITO),
   ECH(DUALIST),
   ECH(THRICE),
   ECH(GOING_HD),
   ECH(REAL_ESTATE_MOGUL),
   ECH(MAXIMUM_DEFINITION),
   ECH(TERMINOLOGIST),
   ECH(LIFE_ON_THE_EDGE),
   ECH(BILINGUAL),
   ECH(POLYGLOT),
   ECH(GADGETEER),
   ECH(INSPECTOR_GADGET),
   ECH(GADGET_HACKWRENCH),
   ECH(LAST)
} Echievement_Id;

typedef struct Echievement
{
   EINA_INLIST;
   Echievement_Id id;
   Etrophy_Trophy *trophy;
   Eina_List *handlers;
   Ecore_Cb mouse_hook;
   Eina_Bool bh_handlers : 1; //handlers are border hooks
} Echievement;

typedef void (*Echievement_Init_Cb)(Echievement*);

typedef struct Mod
{
   E_Config_Dialog *cfd;
   E_Module *module;
   Eina_Hash *trophies; // for fast lookups
   Echievement *trophies_list; // head for iterating
   Eina_Inlist *itr; // current iterator position if needed
   struct
   {
      int x, y;
      int dx, dy;
      Eina_List *hooks;
   } mouse;
   Eina_Bool module_init_end : 1;
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

unsigned int ech_bindings_check_keys(void);
unsigned int ech_bindings_check_mouse(void);
unsigned int ech_bindings_check_wheel(void);
unsigned int ech_bindings_check_edge(void);
unsigned int ech_bindings_check_acpi(void);
unsigned int ech_bindings_check_signal(void);

EINTERN Eina_List *ech_language_enumerate(void);
#endif
