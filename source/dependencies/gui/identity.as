enum EUserRole {
    USER,
    BETA,
    DEVELOPER
}

namespace identity {
    EUserRole role = EUserRole::USER;

    dictionary roles;

    void initialize() {
        roles.set("underscore", EUserRole::DEVELOPER);

        roles.set("draxle", EUserRole::BETA);
        roles.set("touka123", EUserRole::BETA);
        roles.set("Josh", EUserRole::BETA);
        roles.set("promethyzx", EUserRole::BETA);
        roles.set("aat", EUserRole::BETA);
        roles.set("bay", EUserRole::BETA);
        roles.set("GSP", EUserRole::BETA);
        roles.set("Wtf", EUserRole::BETA);


        int r;
        if (roles.get(globals::username, r))
            role = EUserRole(r);
    }

    string get_role_name() {
        switch (role) {
            case EUserRole::DEVELOPER: return "developer";
            case EUserRole::BETA: return "beta";
        }
        return "";
    }

    color_t get_role_color() {
        switch (role) {
            case EUserRole::DEVELOPER: return color_t(255, 80, 80, 255);
            case EUserRole::BETA: return color_t(80, 180, 255, 255);
        }
        return gui::colors::text_active;
    }
}