class StepMove {
    public: int step;
    public: char dir;
    public: int speed;
    public: bool hold;
    public: StepMove(int step, char dir, int speed, bool hold) {
        this->step = step;
        this->dir = dir;
        this->speed = speed;
        this->hold = hold;
    }
};
