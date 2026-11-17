#pragma once

#include "Entries.h"
#include "FitnessGoal.h"
#include "UserProfile.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace calorix {

    class User {
    protected:
        unsigned userId;
        std::string username;
        std::string password;
        UserProfile profile;

    public:
        User();
        User(unsigned userId, const std::string& username, const std::string& password, const UserProfile& profile);
        virtual ~User() = default;

        unsigned getUserId() const;
        const std::string& getUsername() const;
        const std::string& getPassword() const;
        const UserProfile& getProfile() const;
        UserProfile& getProfile();

        bool checkPassword(const std::string& password) const;

        virtual bool isAdmin() const = 0;
        virtual std::string getRole() const = 0;
        virtual void help(std::ostream& os) const = 0;
    };

    class Trainee : public User {
    private:
        std::vector<FoodEntry> foodDiary;
        std::vector<ExerciseEntry> exerciseDiary;
        FitnessGoal goal;
        std::vector<const Exercise*> favoriteExercises;

    public:
        Trainee();
        Trainee(unsigned userId, const std::string& username, const std::string& password, const UserProfile& profile);

        bool isAdmin() const override;
        std::string getRole() const override;
        void help(std::ostream& os) const override;

        const std::vector<FoodEntry>& getFoodDiary() const;
        const std::vector<ExerciseEntry>& getExerciseDiary() const;
        const FitnessGoal& getGoal() const;
        const std::vector<const Exercise*>& getFavoriteExercises() const;

        void setGoals(const FitnessGoal& goal);
        void addLoadedFoodEntry(const FoodEntry& entry);
        void addLoadedExerciseEntry(const ExerciseEntry& entry);
        void addLoadedFavorite(const Exercise* exercise);

        void logFood(unsigned entryId, const Food* food, double quantityGrams, const Date& date);
        void logExercise(unsigned entryId, const Exercise* exercise, unsigned durationMinutes, const Date& date);

        void viewDailySummary(const Date& date, std::ostream& os) const;
        void viewProgress(std::ostream& os) const;

        double calculateBMI() const;
        double calculateBMR() const;

        void generateWorkoutPlan(const std::vector<std::unique_ptr<Exercise>>& exercises,
            unsigned durationMinutes,
            std::ostream& os) const;

        void addToFavorites(const Exercise* exercise);
        void viewFavorites(std::ostream& os) const;
    };

    class Admin : public User {
    public:
        Admin();
        Admin(unsigned userId, const std::string& username, const std::string& password, const UserProfile& profile);

        bool isAdmin() const override;
        std::string getRole() const override;
        void help(std::ostream& os) const override;
    };

}
