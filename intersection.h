#define HORIZONTAL 1
#define VERTICAL 0

class Intersection {
    public:
    Intersection(int x, int y, bool l, int len, int ml);
    Intersection(int x, int y, int ml);
    void checkAndSwitch(); //for local simulating
    void update(int lightFromCloud);
    bool getLight();

    private:
    int locx;
    int locy;
    bool light;
    int lenOfCurLight;
    int maxlen;
};