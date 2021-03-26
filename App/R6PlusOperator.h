#ifndef R6PLUSOPERATOR_H
#define R6PLUSOPERATOR_H


class R6PlusOperator
{
public:
    static R6PlusOperator& GetInstance();
    int verrifyDongle();
    int checkDongle();

private:
    R6PlusOperator();
    R6PlusOperator(const R6PlusOperator& op);
    R6PlusOperator& operator=(const R6PlusOperator& op);
    ~R6PlusOperator();

private:
    //设备句柄
    int m_hic;
};

#endif // R6PLUSOPERATOR_H
