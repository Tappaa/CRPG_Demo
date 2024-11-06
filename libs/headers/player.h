struct player_stats {
    char character_symbol[5];
    int hp;
    int hp_plus_per_level;

    int mp;
    int mp_plus_per_level;

    int atk;
    int atk_plus_per_level;

    int def;
    int def_plus_per_level;

    int level;
    int max_level;

    int critical_chance;
    int critical_plus_per_level;
    int critical_damage_multiplier;
};

extern struct player_stats player;

void initPlayer();
void levelUp();