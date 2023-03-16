#include <iostream>
#include <vector>
#include <memory>

class Ant {
public:
    Ant(int food) : food_(food) {}

    virtual ~Ant() {}

    virtual void eat(int food) {
        food_ += food;
    }

    virtual int getFood() const {
        return food_;
    }

private:
    int food_;
};

class Larva : public Ant {
public:
    Larva(int food) : Ant(food) {}

    void transform() {}

private:
};

class Soldier : public Ant {
public:
    Soldier(int food, int pests) : Ant(food), pests_(pests) {}

    int getPests() const {
        return pests_;
    }

    void eat(int food) override {
        Ant::eat(food);
    }

private:
    int pests_;
};

class Policeman : public Ant {
public:
    Policeman(int food, int increase) : Ant(food), increase_(increase) {}

    int getIncrease() const {
        return increase_;
    }

    void eat(int food) override {
        Ant::eat(food);
    }

private:
    int increase_;
};

class Worker : public Ant {
public:
    Worker(int food, int efficiency) : Ant(food), efficiency_(efficiency) {}

    int getEfficiency() const {
        return efficiency_;
    }

    void eat(int food) override {
        Ant::eat(food);
    }

private:
    int efficiency_;
};

class Womb : public Ant {
public:
    Womb(int food, int larvae) : Ant(food), larvae_(larvae) {}

    int getLarvae() const {
        return larvae_;
    }

    void eat(int food) override {
        Ant::eat(food);
    }

private:
    int larvae_;
};

class Anthill {
public:
    Anthill(int foodStore, int wombFood, int soldiers, int policemen, int workers, int pests, int larvae)
        : foodStore_(foodStore), womb_(wombFood, larvae), soldiers_(soldiers), policemen_(policemen),
        workers_(workers), pests_(pests), days_(0)
    {
        for (int i = 0; i < soldiers_; ++i) {
            ants_.emplace_back(new Soldier(0, pests_ / soldiers_));
        }
        for (int i = 0; i < policemen_; ++i) {
            ants_.emplace_back(new Policeman(0, soldiers_ / policemen_));
        }
        for (int i = 0; i < workers_; ++i) {
            ants_.emplace_back(new Worker(0, policemen_ / workers_));
        }
        for (int i = 0; i < larvae; ++i) {
            ants_.emplace_back(new Larva(0));
        }
    }

    void run(int days) {
        for (int i = 0; i < days; ++i) {
            std::cout << "Day " << days_ << ":\n";
            for (auto& ant : ants_) {
                ant->eat(1);
                if (auto* larva = dynamic_cast<Larva*>(ant.get())) {
                    if (larva->getFood() >= 5) {
                        larva->transform();
                        switch (days_ % 4) {
                        case 0: { ants_.emplace_back(new Soldier(0, pests_ / soldiers_)); break; }
                        case 1: { ants_.emplace_back(new Policeman(0, soldiers_ / policemen_)); break; }
                        case 2: { ants_.emplace_back(new Worker(0, policemen_ / workers_)); break; }
                        case 3: { ants_.emplace_back(new Larva(0)); break; }
                        }
                    }
                }
                else if (auto* soldier = dynamic_cast<Soldier*>(ant.get())) {
                    if (soldier->getFood() >= soldier->getPests()) {
                        soldier->eat(-soldier->getPests());
                        pests_ -= soldier->getPests();
                    }
                }
                else if (auto* policeman = dynamic_cast<Policeman*>(ant.get())) {
                    if (foodStore_ > 0) {
                        int food = foodStore_ * policeman->getIncrease() / 100;
                        foodStore_ -= food;
                        policeman->eat(food);
                    }
                }
                else if (auto* worker = dynamic_cast<Worker*>(ant.get())) {
                    if (foodStore_ > 0) {
                        int food = foodStore_ > worker->getEfficiency() ? worker->getEfficiency() : foodStore_;
                        foodStore_ -= food;
                        worker->eat(food);
                    }
                }
                else if (auto* womb = dynamic_cast<Womb*>(ant.get())) {
                    if (foodStore_ >= womb->getFood() && womb->getLarvae() > 0) {
                        foodStore_ -= womb->getFood();
                        ants_.emplace_back(new Larva(0));
                        womb->eat(womb->getFood());
                        womb->eat(-1);
                    }
                }
            }
            std::cout << "Food store: " << foodStore_ << "\n";
            std::cout << "Number of pests: " << pests_ << "\n";
            std::cout << "Number of soldiers: " << countAntsOfType<Soldier>() << "\n";
            std::cout << "Number of policemen: " << countAntsOfType<Policeman>() << "\n";
            std::cout << "Number of workers: " << countAntsOfType<Worker>() << "\n";
            std::cout << "Number of larvae: " << countAntsOfType<Larva>() << "\n";
            std::cout << "---------------------------\n";
            ++days_;
        }
    }

private:
    int foodStore_;
    Womb womb_;
    int soldiers_;
    int policemen_;
    int workers_;
    int pests_;
    int days_;
    std::vector<std::unique_ptr<Ant>> ants_;

    template <typename T>
    int countAntsOfType() const {
        int count = 0;
        for (const auto& ant : ants_) {
            if (dynamic_cast<const T*>(ant.get())) {
                ++count;
            }
        }
        return count;
    }
};
