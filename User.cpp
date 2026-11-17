#include "User.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ostream>

namespace calorix {

    User::User() : userId(0), username(""), password(""), profile() {}

    User::User(unsigned userId, const std::string& username, const std::string& password, const UserProfile& profile)
        : userId(userId), username(username), password(password), profile(profile) {
    }

    unsigned User::getUserId() const {
        return userId;
    }

    const std::string& User::getUsername() const {
        return username;
    }

    const std::string& User::getPassword() const {
        return password;
    }

    const UserProfile& User::getProfile() const {
        return profile;
    }

    UserProfile& User::getProfile() {
        return profile;
    }

    bool User::checkPassword(const std::string& password) const {
        return this->password == password;
    }

    Trainee::Trainee() : User(), foodDiary(), exerciseDiary(), goal(), favoriteExercises() {}

    Trainee::Trainee(unsigned userId, const std::string& username, const std::string& password, const UserProfile& profile)
        : User(userId, username, password, profile), foodDiary(), exerciseDiary(), goal(), favoriteExercises() {
    }

    bool Trainee::isAdmin() const {
        return false;
    }

    std::string Trainee::getRole() const {
        return "TRAINEE";
    }

    void Trainee::help(std::ostream& os) const {
        os << "Trainee commands:\n"
            << "  set-goals <WEIGHT_LOSS|BULKING|MAINTENANCE> <target-value(kg)> <deadline dd.mm.yyyy>\n"
            << "  log-food <food-name> <quantity-grams>\n"
            << "  log-exercise <exercise-name> <duration-minutes>\n"
            << "  view-daily-summary [date dd.mm.yyyy]\n"
            << "  view-progress\n"
            << "  calculate-bmi\n"
            << "  calculate-bmr\n"
            << "  generate-workout-plan <duration-minutes>\n"
            << "  add-to-favorites <exercise-name>\n"
            << "  view-favorites\n"
            << "  logout\n"
            << "  end\n";
    }

    const std::vector<FoodEntry>& Trainee::getFoodDiary() const {
        return foodDiary;
    }

    const std::vector<ExerciseEntry>& Trainee::getExerciseDiary() const {
        return exerciseDiary;
    }

    const FitnessGoal& Trainee::getGoal() const {
        return goal;
    }

    const std::vector<const Exercise*>& Trainee::getFavoriteExercises() const {
        return favoriteExercises;
    }

    void Trainee::setGoals(const FitnessGoal& goal) {
        this->goal = goal;
    }

    void Trainee::addLoadedFoodEntry(const FoodEntry& entry) {
        foodDiary.push_back(entry);
    }

    void Trainee::addLoadedExerciseEntry(const ExerciseEntry& entry) {
        exerciseDiary.push_back(entry);
    }

    void Trainee::addLoadedFavorite(const Exercise* exercise) {
        if (exercise) {
            favoriteExercises.push_back(exercise);
        }
    }

    void Trainee::logFood(unsigned entryId, const Food* food, double quantityGrams, const Date& date) {
        if (!food) {
            std::cout << "Food does not exist.\n";
            return;
        }

        if (quantityGrams <= 0) {
            std::cout << "Quantity must be positive.\n";
            return;
        }

        foodDiary.emplace_back(entryId, food, quantityGrams, date);
        std::cout << "Food logged successfully.\n";
    }

    void Trainee::logExercise(unsigned entryId, const Exercise* exercise, unsigned durationMinutes, const Date& date) {
        if (!exercise) {
            std::cout << "Exercise does not exist.\n";
            return;
        }

        if (durationMinutes == 0) {
            std::cout << "Duration must be positive.\n";
            return;
        }

        exerciseDiary.emplace_back(entryId, exercise, durationMinutes, date);
        std::cout << "Exercise logged successfully.\n";
    }

    void Trainee::viewDailySummary(const Date& date, std::ostream& os) const {
        double totalCalories = 0;
        double totalProtein = 0;
        double totalCarbs = 0;
        double totalFat = 0;
        double totalBurnedCalories = 0;

        for (const FoodEntry& entry : foodDiary) {
            if (entry.getDate() == date) {
                totalCalories += entry.getCalories();
                totalProtein += entry.getProtein();
                totalCarbs += entry.getCarbs();
                totalFat += entry.getFat();
            }
        }

        for (const ExerciseEntry& entry : exerciseDiary) {
            if (entry.getDate() == date) {
                totalBurnedCalories += entry.getBurnedCalories();
            }
        }

        os << std::fixed << std::setprecision(2);
        os << "Daily summary for " << date.toString() << ":\n";
        os << "  Consumed calories: " << totalCalories << " kcal\n";
        os << "  Protein: " << totalProtein << " g\n";
        os << "  Carbs: " << totalCarbs << " g\n";
        os << "  Fat: " << totalFat << " g\n";
        os << "  Burned calories: " << totalBurnedCalories << " kcal\n";
        os << "  Net balance: " << totalCalories - totalBurnedCalories << " kcal\n";
    }

    void Trainee::viewProgress(std::ostream& os) const {
        goal.print(os);

        if (goal.getGoalType() == GoalType::WEIGHT_LOSS) {
            const double currentWeight = profile.getWeight();
            const double targetWeight = goal.getTargetValue();

            if (currentWeight > targetWeight) {
                os << "Remaining: " << currentWeight - targetWeight << " kg to target weight.\n";
            }
            else {
                os << "Target weight reached or passed.\n";
            }
        }
        else if (goal.getGoalType() == GoalType::BULKING) {
            os << "Target calorie surplus: " << goal.getTargetValue() << " kcal.\n";
        }
        else {
            os << "Maintenance goal.\n";
        }
    }

    double Trainee::calculateBMI() const {
        const double heightInMeters = profile.getHeight() / 100.0;

        if (heightInMeters <= 0) {
            return 0;
        }

        return profile.getWeight() / (heightInMeters * heightInMeters);
    }

    double Trainee::calculateBMR() const {
        const double weight = profile.getWeight();
        const double height = profile.getHeight();
        const unsigned age = profile.getAge();

        if (profile.getGender() == Gender::MALE) {
            return 10 * weight + 6.25 * height - 5 * age + 5;
        }

        return 10 * weight + 6.25 * height - 5 * age - 161;
    }

    void Trainee::generateWorkoutPlan(const std::vector<std::unique_ptr<Exercise>>& exercises,
        unsigned durationMinutes,
        std::ostream& os) const {
        if (durationMinutes == 0) {
            os << "Duration must be positive.\n";
            return;
        }

        if (exercises.empty()) {
            os << "No exercises available.\n";
            return;
        }

        const unsigned n = static_cast<unsigned>(exercises.size());
        std::vector<std::vector<double>> dp(n + 1, std::vector<double>(durationMinutes + 1, 0));

        for (unsigned i = 1; i <= n; ++i) {
            const Exercise* exercise = exercises[i - 1].get();
            const unsigned weight = exercise->getDurationMinutes();
            const double value = exercise->getKnapsackValue();

            for (unsigned t = 0; t <= durationMinutes; ++t) {
                dp[i][t] = dp[i - 1][t];

                if (weight <= t) {
                    dp[i][t] = std::max(dp[i][t], dp[i - 1][t - weight] + value);
                }
            }
        }

        std::vector<const Exercise*> chosen;
        unsigned remainingTime = durationMinutes;

        for (unsigned i = n; i >= 1; --i) {
            const Exercise* exercise = exercises[i - 1].get();
            const unsigned weight = exercise->getDurationMinutes();
            const double value = exercise->getKnapsackValue();

            if (remainingTime >= weight && dp[i][remainingTime] == dp[i - 1][remainingTime - weight] + value) {
                chosen.push_back(exercise);
                remainingTime -= weight;
            }
        }

        if (chosen.empty()) {
            os << "No exercise fits in the requested duration.\n";
            return;
        }

        std::reverse(chosen.begin(), chosen.end());

        double totalCalories = 0;
        unsigned totalMinutes = 0;

        os << "Optimal workout plan by 0-1 knapsack:\n";

        for (const Exercise* exercise : chosen) {
            const double calories = exercise->getKnapsackValue();
            totalCalories += calories;
            totalMinutes += exercise->getDurationMinutes();

            os << "  - " << exercise->getName()
                << " | " << exercise->getDurationMinutes() << " min"
                << " | " << calories << " kcal\n";
        }

        os << "Total time: " << totalMinutes << " min\n";
        os << "Total expected burned calories: " << totalCalories << " kcal\n";
    }

    void Trainee::addToFavorites(const Exercise* exercise) {
        if (!exercise) {
            std::cout << "Exercise does not exist.\n";
            return;
        }

        if (std::find(favoriteExercises.begin(), favoriteExercises.end(), exercise) != favoriteExercises.end()) {
            std::cout << "Exercise is already in favorites.\n";
            return;
        }

        favoriteExercises.push_back(exercise);
        std::cout << "Exercise added to favorites.\n";
    }

    void Trainee::viewFavorites(std::ostream& os) const {
        if (favoriteExercises.empty()) {
            os << "No favorite exercises.\n";
            return;
        }

        os << "Favorite exercises:\n";
        for (const Exercise* exercise : favoriteExercises) {
            if (exercise) {
                os << "  - ";
                exercise->print(os);
            }
        }
    }

    Admin::Admin() : User() {}

    Admin::Admin(unsigned userId, const std::string& username, const std::string& password, const UserProfile& profile)
        : User(userId, username, password, profile) {
    }

    bool Admin::isAdmin() const {
        return true;
    }

    std::string Admin::getRole() const {
        return "ADMIN";
    }

    void Admin::help(std::ostream& os) const {
        os << "Admin commands:\n"
            << "  add-food <name> <calories/100g> <protein/100g> <carbs/100g> <fat/100g>\n"
            << "  add-exercise <name> <calories/hour> <ARMS|CHEST|LEGS|CARDIO|BACK|SHOULDERS|CORE> [duration-minutes]\n"
            << "  update-food <food-name> <new-calories/100g>\n"
            << "  block-user <username>\n"
            << "  list-foods\n"
            << "  list-exercises\n"
            << "  logout\n"
            << "  end\n";
    }

}
