#pragma once

#include "Enums.h"

namespace calorix {

    class UserProfile {
    private:
        unsigned age;
        double weight;
        double height;
        Gender gender;
        ActivityLevel activityLevel;

    public:
        UserProfile();
        UserProfile(unsigned age, double weight, double height, Gender gender, ActivityLevel activityLevel);

        unsigned getAge() const;
        double getWeight() const;
        double getHeight() const;
        Gender getGender() const;
        ActivityLevel getActivityLevel() const;

        void setAge(unsigned age);
        void setWeight(double weight);
        void setHeight(double height);
        void setGender(Gender gender);
        void setActivityLevel(ActivityLevel activityLevel);
    };


}
