//------------
// SinkLamp
//------------
int const sinkTheatrePercent = 10;
int const sinkEarlyAwakePercent = 5;

void setSinkLamp() {
    int percent = 0;
    int current = value("SinkLamp");

    if(is("Cleaning")) {
        percent = 100;

    } else if(is("Sink")) {
        if(is("Theatre")) {
            percent = sinkTheatrePercent;
        } else {
            percent = value("Sink");
        }

    } else switch(partOfDay()) {
        case Evening:
            if(is("Theatre")) {
                percent = sinkTheatrePercent;
            } else {
                percent = 100;
            }   
            break;

        case AwakeEarly:
            percent = sinkEarlyAwakePercent;
            break;

        case Retiring:
        case Asleep:
        case Morning:
        case Afternoon:
            percent = 0;
    }
    if(percent != current) {
        set("SinkLamp", percent);
    }
}
