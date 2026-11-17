#pragma once

#include "Exercise.h"
#include "Food.h"
#include "User.h"

#include <memory>
#include <string>
#include <vector>

namespace calorix {

    class Calorix {
    private:
        std::vector<std::unique_ptr<User>> users;
        std::vector<std::unique_ptr<Food>> foods;
        std::vector<std::unique_ptr<Exercise>> exercises;

        User* currentUser;

        unsigned nextUserId;
        unsigned nextFoodId;
        unsigned nextExerciseId;
        unsigned nextEntryId;

        std::string dataFileName;

    private:
        User* findUserByUsername(const std::string& username);
        const User* findUserByUsername(const std::string& username) const;

        Food* findFoodByName(const std::string& name);
        const Food* findFoodByName(const std::string& name) const;
        Food* findFoodById(unsigned foodId);
        const Food* findFoodById(unsigned foodId) const;

        Exercise* findExerciseByName(const std::string& name);
        const Exercise* findExerciseByName(const std::string& name) const;
        Exercise* findExerciseById(unsigned exerciseId);
        const Exercise* findExerciseById(unsigned exerciseId) const;

        Trainee* getCurrentTrainee();
        const Trainee* getCurrentTrainee() const;
        Admin* getCurrentAdmin();

        bool isUsernameTaken(const std::string& username) const;
        void createDefaultAdmin();
        void executeCommand(const std::string& line);

    public:
        explicit Calorix(const std::string& dataFileName = "calorix_data.txt");

        void run();

        void registerUser(const std::string& username,
            const std::string& password,
            unsigned age,
            double weight,
            double height,
            Gender gender,
            ActivityLevel activityLevel = ActivityLevel::MODERATE);

        void login(const std::string& username, const std::string& password);
        void logout();

        void addFood(const std::string& name,
            double calories,
            double protein,
            double carbs,
            double fat);

        void addExercise(const std::string& name,
            double caloriesBurnedPerHour,
            MuscleGroup muscleGroup,
            unsigned durationMinutes = 30);

        void updateFood(const std::string& foodName, double newCalories);
        void blockUser(const std::string& username);

        void logFood(const std::string& foodName, double quantityGrams);
        void logExercise(const std::string& exerciseName, unsigned durationMinutes);

        void setGoals(GoalType goalType, double targetValue, const Date& deadline);

        void viewDailySummary(const Date& date = Date::today()) const;
        void viewProgress() const;
        void calculateBMI() const;
        void calculateBMR() const;
        void generateWorkoutPlan(unsigned durationMinutes) const;
        void addToFavorites(const std::string& exerciseName);
        void viewFavorites() const;

        void listFoods() const;
        void listExercises() const;
        void help() const;

        bool loadFromFile();
        bool saveToFile() const;
    };

}