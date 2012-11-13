/* TO ADD A NEW ECHIEVEMENT:
 *
 * 1) Append new Echievement_Id before ECHIEVEMENT_ID_LAST
 * 2) Add Echievement_Hide_States
 * 3) Add Echievement_Goals
 * 4) Add Echievement_Strings
 * 5) Add Echievement_Descs
 * 6) Add Echievement_Callbacks
 * 7) Add related ECH_INIT and ECH_EH functions in echievements.c
 */

static Etrophy_Trophy_Visibility Echievement_Hide_States[] =
{
   [ECHIEVEMENT_ID_SHELF_POSITIONS] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECHIEVEMENT_ID_NOTHING_ELSE_MATTERS] = ETROPHY_TROPHY_STATE_VISIBLE,
};

/* ec may be NULL */
static unsigned int Echievement_Goals[] =
{
                                      /* LAST - first possible shelf orient */
   [ECHIEVEMENT_ID_SHELF_POSITIONS] = E_GADCON_ORIENT_LAST - E_GADCON_ORIENT_LEFT,
   [ECHIEVEMENT_ID_NOTHING_ELSE_MATTERS] = 1,
};

static const char *const Echievement_Strings[] =
{
   [ECHIEVEMENT_ID_SHELF_POSITIONS] = "Shelves Everywhere",
   [ECHIEVEMENT_ID_NOTHING_ELSE_MATTERS] = "Nothing Else Matters",
};

static const char *const Echievement_Descs[] =
{
   [ECHIEVEMENT_ID_SHELF_POSITIONS] = "Have a shelf in every position",
   [ECHIEVEMENT_ID_NOTHING_ELSE_MATTERS] = "Use E17 with no shelves",
};

#define ECB(NAME) void echievement_init_cb_##NAME(Echievement *ec)

ECB(SHELF_POSITIONS);
ECB(NOTHING_ELSE_MATTERS);

#undef ECB
#define ECB(NAME) [ECHIEVEMENT_ID_##NAME] = echievement_init_cb_##NAME

Echievement_Init_Cb Echievement_Callbacks[] =
{
   ECB(SHELF_POSITIONS),
   ECB(NOTHING_ELSE_MATTERS),
   NULL
};

#undef ECB
