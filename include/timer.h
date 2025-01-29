#ifndef _TIMER
#define _TIMER

#include <cstdint>
class Timer
{
public:
    uint8_t value { 0 };
    void decrement() {
        if (value > 0)
        {
            value--;
            if (sound)
            {
                // Play a beep sound
            }
        }
    }
    Timer(bool sound) { this->sound = sound; }
private:
    bool sound;
};

#endif
