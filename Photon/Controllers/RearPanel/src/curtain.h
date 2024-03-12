//------------
// curtain
//------------
void setCurtain() {
    int current = value("curtain");

    switch(partOfDay()) {
        case Evening:
            break;
        case Retiring:
        case Asleep:
            if(current > 0) {
                set("curtain", 0);
            }
            break;
        case AwakeEarly:
        case Morning:
        case Afternoon:
            break;
    }
}
