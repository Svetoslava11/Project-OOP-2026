#include "UserProfile.h"

namespace calorix {

    UserProfile::UserProfile()
        : age(0), weight(0), height(0), gender(Gender::FEMALE), activityLevel(ActivityLevel::MODERATE) {
    }

    UserProfile::UserProfile(unsigned age, double weight, double height, Gender gender, ActivityLevel activityLevel)
        : age(age), weight(weight), height(height), gender(gender), activityLevel(activityLevel) {
    }

    unsigned UserProfile::getAge() const {
        return age;
    }

    double UserProfile::getWeight() const {
        return weight;
    }

    double UserProfile::getHeight() const {
        return height;
    }

    Gender UserProfile::getGender() const {
        return gender;
    }

    ActivityLevel UserProfile::getActivityLevel() const {
        return activityLevel;
    }

    void UserProfile::setAge(unsigned age) {
        this->age = age;
    }

    void UserProfile::setWeight(double weight) {
        if (weight > 0) {
            this->weight = weight;
        }
    }

    void UserProfile::setHeight(double height) {
        if (height > 0) {
            this->height = height;
        }
    }

    void UserProfile::setGender(Gender gender) {
        this->gender = gender;
    }

    void UserProfile::setActivityLevel(ActivityLevel activityLevel) {
        this->activityLevel = activityLevel;
    }

}