#ifndef ROCKEYARMOPERATOR_H
#define ROCKEYARMOPERATOR_H


class RockeyArmOperator
{
public:
    static RockeyArmOperator& GetInstance();
    int verrifyDongle();

private:
    RockeyArmOperator();
    RockeyArmOperator(const RockeyArmOperator& op);
    RockeyArmOperator& operator=(const RockeyArmOperator& op);
    ~RockeyArmOperator();
};

#endif // ROCKEYARMOPERATOR_H
