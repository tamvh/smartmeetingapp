import QtQuick 2.0

Item  {
    id: timerCountdown
    anchors.left: labelTimer.right

    property int fontSize: 15
    property double delayTimer: 90000; //1000 milisecond = 1 second
    property double startTime: 0
    property int minutesElapsed: 0
    property int secondsElapsed: 0
    property int parentId:0

    function restartTimerCounter()  {
        timerCountdown.startTime = 0;
        timerCountdown.minutesElapsed = 0;
        timerCountdown.secondsElapsed = 0;

        elapsedTimer.running = true;
    }

    function stopTimerCounter() {
        elapsedTimer.running = false;
    }

    function timeChanged()  {
        if(timerCountdown.startTime<=0)
        {
            //returns the number of milliseconds since the epoch (1970-01-01T00:00:00Z);
            timerCountdown.startTime = new Date().getTime() + delayTimer;
        }
        var timeCounter = (startTime - new Date().getTime())/1000;

        if (timeCounter <= 0) {
            startTime = 0;
            if (parentId === 1) {
                qrdialog.close();
            }
        }

        timerCountdown.minutesElapsed = (timeCounter)/60;
        timerCountdown.secondsElapsed = (timeCounter - (timerCountdown.minutesElapsed*60));
    }

    Timer  {
        id: elapsedTimer
        interval: 1000;
        running: false;
        repeat: true;
        onTriggered: timerCountdown.timeChanged()
    }

    Text {
        id: counterText
        // conver one-number-digit >> two-number-digit; 9 >> 09 ...
        text: ("0" + timerCountdown.minutesElapsed).slice(-2) + ":" + ("0"+timerCountdown.secondsElapsed).slice(-2)
        font.pixelSize: fontSize
    }
}
