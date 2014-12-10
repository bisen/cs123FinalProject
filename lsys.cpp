#include "lsys.h"

Rule::Rule(char in, char *out) {
    this->in = in;
    this->out = new char[strlen(out)];

    this->length = strlen(out);
    memcpy(this->out, out, length * sizeof(char));
}

Rule::~Rule() {
    delete[] out;
}

void Rule::apply(char input, char **result, bool &happened) {
    if(input == in) {
        memcpy(*result, out, length * sizeof(char));
        *result += length;
        happened = true;
    }
}


LSys::LSys()
{
    m_rules = new QList<Rule *>();
}

LSys::LSys(QList<Rule *> rs) {
    m_rules = new QList<Rule *>(rs);
}

LSys::~LSys() {
    delete m_rules;
}

void LSys::addRule(Rule *r) {
    m_rules->append(r);
}

char *LSys::generateStep(char *current, int n) {
    if(n <= 0) return current;
    int currentlength = strlen(current);
    int nextlength = 12 * currentlength;
    char *result = new char[nextlength];
    memset(result, '\0', nextlength);
    char *result_idx = result;

    int currentlen = strlen(current);

    for(int i = 0; i < currentlen; i++) {
        bool happened = false;
        for(int j = 0; j < m_rules->length(); j++) {
            m_rules->at(j)->apply(current[i], &result_idx, happened);
        }
        if(!happened) {
            *result_idx = current[i];
            result_idx += 1;
        }
    }

    delete[] current;
    printf("%s\n", result);
    return generateStep(result, n-1);
}

char *LSys::generate(int n, char *initial) {
    char *input = new char[strlen(initial) + 1];
    memset(input, '\0', strlen(initial) + 1);
    memcpy(input, initial, strlen(initial));

    return generateStep(input, n);
}
