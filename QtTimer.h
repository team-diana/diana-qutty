#ifndef QTTIMER_H
#define QTTIMER_H

#include <QObject>
#include <QTimerEvent>
#include <QTime>
extern "C" {
#include "putty.h"
}

class QtTimer : public QObject {
    Q_OBJECT

    int timerId;
    long nextTick;
    QTime countTimer;

public:
    QtTimer()
    {
        timerId = - 1;
        countTimer.start();
    }

    void startTimerForTick(long nextTick)
    {
        long ticks = nextTick - countTimer.elapsed();
        if (ticks <= 0) ticks = 1;	       /* just in case */
        if (timerId != -1)
            this->killTimer(timerId);
        timerId = this->startTimer(ticks);
        this->nextTick = nextTick;
    }

protected:
    void timerEvent(QTimerEvent *event)
    {
        long next;
        killTimer(timerId);

        // only one timer is active at any point of time
        qutty_assert(event->timerId() == timerId);

        if (run_timers(this->nextTick, &next)) {
            startTimerForTick(next);
        }
    }
};

#endif // QTTIMER_H
