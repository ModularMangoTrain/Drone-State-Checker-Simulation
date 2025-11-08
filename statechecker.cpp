#include <iostream>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

class droneStateChecker{
public:

    enum class state{
        OFF, BOOTING, ON, CALIBRATING, TAKING_OFF, HOVERING, FLYING, BLUETOOTH_CHECKER, THERMAL_SCANNER, DAMAGED, LANDING, ERROR, SHUTTING_DOWN
    };

private:

    state currentState;
    int batteryLevel;
    bool gpsSignal;
    int altitude;
    int positionX;
    int positionY;

    string stateToString(state midhunmoment){
        switch (midhunmoment){
            case state::OFF: return "OFF";
            case state::BOOTING: return "BOOTING ON";
            case state::ON: return "ON";
            case state::CALIBRATING: return "calibrating brah";
            case state::TAKING_OFF: return "will try not to go into the stratosphere (taking off)";
            case state::HOVERING: return "HOVERING";
            case state::FLYING: return "flying";
            case state::BLUETOOTH_CHECKER: return "pinging nearby people";
            case state::THERMAL_SCANNER: return "SCANNING THERMAL";
            case state::DAMAGED: return "im broken";
            case state::LANDING: return "landing";
            case state::ERROR: return "returning";
            case state::SHUTTING_DOWN: return "bye bye(shutting down)";
            default: return "idk you actually broke the code somehow";

        }



    }
    bool canChangeState(state newState) {

        switch (currentState) {
            case state::OFF:
                return newState == state::BOOTING;

            case state::BOOTING:
                return newState == state::ON || newState == state::ERROR;

            case state::ON:
                return newState == state::CALIBRATING || newState == state::TAKING_OFF || newState == state::BLUETOOTH_CHECKER || newState == state::THERMAL_SCANNER ||newState == state::SHUTTING_DOWN;

            case state::CALIBRATING:
                return newState == state::ON || newState == state::ERROR;

            case state::TAKING_OFF:
                return newState == state::HOVERING || newState == state::ERROR;

            case state::HOVERING:
                return newState == state::FLYING || newState == state::LANDING;

            case state::FLYING:
                return newState == state::HOVERING || newState == state::LANDING || newState == state::THERMAL_SCANNER || newState == state::DAMAGED;

            case state::BLUETOOTH_CHECKER:
                return newState == state::ON || newState == state::ERROR;

            case state::THERMAL_SCANNER:
                return newState == state::FLYING || newState == state::ON;

            case state::DAMAGED:
                return newState == state::LANDING || newState == state::ERROR;

            case state::LANDING:
                return newState == state::ON || newState == state::DAMAGED;

            case state::ERROR:
                return newState == state::SHUTTING_DOWN || newState == state::ON;

            case state::SHUTTING_DOWN:
                return newState == state::OFF;

            default:
                return false;
        }
    }

    
    void updateDroneState(state newState) {
        switch (newState) {
            case state::FLYING:
                batteryLevel -= 15;
                altitude = 50;
                positionX += 20;
                positionY += 20;
                break;

            case state::HOVERING:
                batteryLevel -= 5;
                altitude = 30;
                break;

            case state::TAKING_OFF:
                batteryLevel -= 10;
                altitude = 10;
                break;

            case state::LANDING:
                batteryLevel -= 5;
                altitude = 0;
                break;

            case state::THERMAL_SCANNER:
                batteryLevel -= 8;
                break;
                
            case state::BLUETOOTH_CHECKER:
                batteryLevel -= 3;
                break;

            case state::BOOTING:
                batteryLevel -= 1;

                currentState = state::ON;
                cout << "Drone is ON";
                break;

            case state::DAMAGED:
                gpsSignal = false;
                batteryLevel -= 25;
                break;
        }
        if (batteryLevel < 0){
            batteryLevel = 0;
        }
    }
    


public:
    droneStateChecker() {
        currentState = state::OFF;
        batteryLevel = 100;
        gpsSignal = true;
        altitude = 0;
        positionX = 0;
        positionY = 0;
    }
    void checkState(){
        cout << "\nDRONE STATUS CHECK SIMULATOR (based off of draft flowchart)" << endl;
        cout << "\ncurrent state: " << stateToString(currentState) << endl;
        cout << "Battery: " << batteryLevel << "%" << endl;
        cout << "GPS Signal: " << (gpsSignal ? "Strong" : "Weak") << endl;
        cout << "Altitude: " << altitude << "m" << endl;
        cout << "Position: (" << positionX << ", " << positionY << ")" << endl;

        if (batteryLevel < 20 && currentState != state::SHUTTING_DOWN) {
            cout << "Battery low" << endl;
        }
        if (!gpsSignal && (currentState == state::FLYING || currentState == state::HOVERING)) {
            cout << "WE CAN GO THAT HIGH/GPS signal lost" << endl;
        }
        if (altitude > 100 && currentState == state::FLYING) {
            cout << "height maxxed (max height exceeded)" << endl;
        }

    }

    bool changeState(state newState){
        if (canChangeState(newState)){
            cout << "going from " << stateToString(currentState) << " to " << stateToString(newState) << endl;
            currentState = newState;
            updateDroneState(newState);

            if (newState == state::TAKING_OFF){
                cout << "taking off to hovering" << endl;
                currentState = state::HOVERING;
                updateDroneState(state::HOVERING);
            }
            return true;
        }
        else {
            cout << "cant change state from " << stateToString(currentState) << " to " << stateToString(newState) << endl;
            return false;
        }
    }

    void emergencyShutdown(){
        cout << "SHUTDOWN" << endl;
        currentState = state::SHUTTING_DOWN;
        updateDroneState(state::SHUTTING_DOWN);
        currentState = state::OFF;
        cout << "Drone is now OFF" << endl;
    }
    
    void menu() {
        cout << "\nCONTROL PANEL" << endl;
        cout << "1. Turn ON (Boot up)" << endl;
        cout << "2. Calibrate" << endl;
        cout << "3. Take Off" << endl;
        cout << "4. Start Flying" << endl;
        cout << "5. Hover" << endl;
        cout << "6. Thermal Scan" << endl;
        cout << "7. Bluetooth Ping" << endl;
        cout << "8. Land" << endl;
        cout << "9. Shutdown" << endl;
        cout << "10. Check Status" << endl;
        cout << "11. Simulate Damage (oh no)" << endl;
        cout << "12. Exit" << endl;
    }


    state getCurrentState() { return currentState; }
    int getBattery() { return batteryLevel; }


};

int main(){
    droneStateChecker drone;

    int choice;

    while (true){
    drone.menu();
    cout << "pick an action to do brotato: ";
    cin >> choice;
    switch (choice) {
            case 1:
                drone.changeState(droneStateChecker::state::BOOTING);
                break;
            case 2:
                drone.changeState(droneStateChecker::state::CALIBRATING);
                break;
            case 3:
                drone.changeState(droneStateChecker::state::TAKING_OFF);
                break;
            case 4:
                drone.changeState(droneStateChecker::state::FLYING);
                break;
            case 5:
                drone.changeState(droneStateChecker::state::HOVERING);
                break;
            case 6:
                drone.changeState(droneStateChecker::state::THERMAL_SCANNER);
                break;
            case 7:
                drone.changeState(droneStateChecker::state::BLUETOOTH_CHECKER);
                break;
            case 8:
                drone.changeState(droneStateChecker::state::LANDING);
                break;
            case 9:
                drone.emergencyShutdown();
                break;
            case 10:
                drone.checkState();
                break;
            case 11:
                drone.changeState(droneStateChecker::state::DAMAGED);
                break;
            case 12:
                cout << "drone off bro" << endl;
                return 0;
            default:
                cout << "Try again" << endl;
                break;
        }

    drone.checkState();

    }
    return 0;
}

