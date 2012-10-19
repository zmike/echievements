
static Etrophy_Trophy_Visibility Echievement_Hide_State[] =
{
   [ECHIEVEMENT_SHELF_POSITIONS] = ETROPHY_TROPHY_STATE_VISIBLE,
};

/* ec may be NULL */
static unsigned int Echievement_Goals[] =
{
                                   /* LAST - first possible shelf orient */
   [ECHIEVEMENT_SHELF_POSITIONS] = E_GADCON_ORIENT_LAST - E_GADCON_ORIENT_LEFT,
};

static const char *const Echievement_Strings[] =
{
   [ECHIEVEMENT_SHELF_POSITIONS] = "shelf_positions",
};

static const char *const Echievement_Descs[] =
{
   [ECHIEVEMENT_SHELF_POSITIONS] = "Have a shelf in every position",
};

#define ECB(NAME) void echievement_init_cb_##NAME(Echievement *ec)

ECB(SHELF_POSITIONS);

#undef ECB
#define ECB(NAME) [ECHIEVEMENT_##NAME] = echievement_init_cb_##NAME

Echievement_Init_Cb Echievement_Callbacks[] =
{
   ECB(SHELF_POSITIONS),
   NULL
};

#undef ECB
