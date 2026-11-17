#pragma once

#include <algorithm>
#include <cctype>
#include <string>

namespace calorix {

    enum class Gender {
        MALE,
        FEMALE
    };

    enum class ActivityLevel {
        SEDENTARY,
        LIGHT,
        MODERATE,
        ACTIVE,
        VERY_ACTIVE
    };

    enum class GoalType {
        WEIGHT_LOSS,
        BULKING,
        MAINTENANCE
    };

    enum class MuscleGroup {
        CHEST,
        BACK,
        LEGS,
        SHOULDERS,
        ARMS,
        CORE,
        CARDIO
    };

    inline std::string toUpper(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
            return static_cast<char>(std::toupper(ch));
            });
        return value;
    }

    inline std::string toString(Gender gender) {
        switch (gender) {
        case Gender::MALE: return "MALE";
        case Gender::FEMALE: return "FEMALE";
        default: return "UNKNOWN";
        }
    }

    inline std::string toString(ActivityLevel level) {
        switch (level) {
        case ActivityLevel::SEDENTARY: return "SEDENTARY";
        case ActivityLevel::LIGHT: return "LIGHT";
        case ActivityLevel::MODERATE: return "MODERATE";
        case ActivityLevel::ACTIVE: return "ACTIVE";
        case ActivityLevel::VERY_ACTIVE: return "VERY_ACTIVE";
        default: return "UNKNOWN";
        }
    }

    inline std::string toString(GoalType type) {
        switch (type) {
        case GoalType::WEIGHT_LOSS: return "WEIGHT_LOSS";
        case GoalType::BULKING: return "BULKING";
        case GoalType::MAINTENANCE: return "MAINTENANCE";
        default: return "UNKNOWN";
        }
    }

    inline std::string toString(MuscleGroup group) {
        switch (group) {
        case MuscleGroup::CHEST: return "CHEST";
        case MuscleGroup::BACK: return "BACK";
        case MuscleGroup::LEGS: return "LEGS";
        case MuscleGroup::SHOULDERS: return "SHOULDERS";
        case MuscleGroup::ARMS: return "ARMS";
        case MuscleGroup::CORE: return "CORE";
        case MuscleGroup::CARDIO: return "CARDIO";
        default: return "UNKNOWN";
        }
    }

    inline bool parseGender(const std::string& text, Gender& out) {
        const std::string value = toUpper(text);
        if (value == "MALE" || value == "M") {
            out = Gender::MALE;
            return true;
        }
        if (value == "FEMALE" || value == "F") {
            out = Gender::FEMALE;
            return true;
        }
        return false;
    }

    inline bool parseActivityLevel(const std::string& text, ActivityLevel& out) {
        const std::string value = toUpper(text);
        if (value == "SEDENTARY") {
            out = ActivityLevel::SEDENTARY;
            return true;
        }
        if (value == "LIGHT") {
            out = ActivityLevel::LIGHT;
            return true;
        }
        if (value == "MODERATE") {
            out = ActivityLevel::MODERATE;
            return true;
        }
        if (value == "ACTIVE") {
            out = ActivityLevel::ACTIVE;
            return true;
        }
        if (value == "VERY_ACTIVE") {
            out = ActivityLevel::VERY_ACTIVE;
            return true;
        }
        return false;
    }

    inline bool parseGoalType(const std::string& text, GoalType& out) {
        const std::string value = toUpper(text);
        if (value == "WEIGHT_LOSS") {
            out = GoalType::WEIGHT_LOSS;
            return true;
        }
        if (value == "BULKING") {
            out = GoalType::BULKING;
            return true;
        }
        if (value == "MAINTENANCE") {
            out = GoalType::MAINTENANCE;
            return true;
        }
        return false;
    }

    inline bool parseMuscleGroup(const std::string& text, MuscleGroup& out) {
        const std::string value = toUpper(text);
        if (value == "CHEST") {
            out = MuscleGroup::CHEST;
            return true;
        }
        if (value == "BACK") {
            out = MuscleGroup::BACK;
            return true;
        }
        if (value == "LEGS") {
            out = MuscleGroup::LEGS;
            return true;
        }
        if (value == "SHOULDERS") {
            out = MuscleGroup::SHOULDERS;
            return true;
        }
        if (value == "ARMS") {
            out = MuscleGroup::ARMS;
            return true;
        }
        if (value == "CORE") {
            out = MuscleGroup::CORE;
            return true;
        }
        if (value == "CARDIO") {
            out = MuscleGroup::CARDIO;
            return true;
        }
        return false;
    }

}