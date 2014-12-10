#ifndef LSYS_H
#define LSYS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <QList>

//Helper class representing a rule for an L-System
class Rule
{
public:
    Rule(char in, char *out);
    virtual ~Rule();

    char in;
    char *out;
    int length;

    //Modify result to contain output of rule if input matches input condition
    void apply(char input, char **result, bool &happened);

};

//Generates Lindenmayer systems given a number of rules
class LSys
{
public:
    LSys();
    LSys(QList<Rule *> rs);
    virtual ~LSys();

    //Add a rule to this system
    void addRule(Rule *r);
    char *generate(int n, char *initial);

protected:

    QList<Rule *> *m_rules;
    int m_length;

    char *generateStep(char *current, int n);
};

#endif // LSYS_H
