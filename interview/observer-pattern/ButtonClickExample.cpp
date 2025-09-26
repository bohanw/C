#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Observer Pattern Implementation
template<typename EventType>
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onEvent(const EventType& event) = 0;
};

template<typename EventType>
class Subject {
private:
    std::vector<Observer<EventType>*> observers_;

public:
    void subscribe(Observer<EventType>* observer) {
        observers_.push_back(observer);
    }

    void unsubscribe(Observer<EventType>* observer) {
        observers_.erase(
            std::remove(observers_.begin(), observers_.end(), observer),
            observers_.end()
        );
    }

    void notify(const EventType& event) {
        for (auto* observer : observers_) {
            observer->onEvent(event);
        }
    }
    
    size_t getObserverCount() const {
        return observers_.size();
    }
};

// Button Click Event
struct ButtonClickEvent {
    int button_id;
    int x, y;
};

class Button : public Subject<ButtonClickEvent> {
private:
    int id_;
    std::string label_;

public:
    explicit Button(int id, const std::string& label = "") 
        : id_(id), label_(label.empty() ? "Button" + std::to_string(id) : label) {}

    void click(int x, int y) {
        std::cout << ">>> " << label_ << " was clicked! <<<" << std::endl;
        ButtonClickEvent event{id_, x, y};
        notify(event);
    }
    
    int getId() const { return id_; }
    const std::string& getLabel() const { return label_; }
};

// Different types of observers
class ButtonObserver : public Observer<ButtonClickEvent> {
private:
    std::string name_;

public:
    explicit ButtonObserver(const std::string& name) : name_(name) {}

    void onEvent(const ButtonClickEvent& event) override {
        std::cout << "[" << name_ << "] Button " << event.button_id 
                  << " clicked at (" << event.x << ", " << event.y << ")" << std::endl;
    }
};

class ClickCounterObserver : public Observer<ButtonClickEvent> {
private:
    int click_count_;

public:
    ClickCounterObserver() : click_count_(0) {}

    void onEvent(const ButtonClickEvent& event) override {
        click_count_++;
        std::cout << "[ClickCounter] Total clicks so far: " << click_count_ 
                  << " (latest from button " << event.button_id << ")" << std::endl;
    }
    
    int getClickCount() const { return click_count_; }
};

class LoggingObserver : public Observer<ButtonClickEvent> {
public:
    void onEvent(const ButtonClickEvent& event) override {
        std::cout << "[Logger] CLICK_EVENT: {button_id:" << event.button_id 
                  << ", x:" << event.x << ", y:" << event.y << "}" << std::endl;
    }
};

class CoordinateTrackerObserver : public Observer<ButtonClickEvent> {
private:
    std::vector<std::pair<int, int>> coordinates_;

public:
    void onEvent(const ButtonClickEvent& event) override {
        coordinates_.emplace_back(event.x, event.y);
        std::cout << "[CoordinateTracker] Stored coordinate (" << event.x 
                  << ", " << event.y << "). Total stored: " << coordinates_.size() << std::endl;
    }
    
    void printAllCoordinates() const {
        std::cout << "[CoordinateTracker] All stored coordinates:" << std::endl;
        for (size_t i = 0; i < coordinates_.size(); ++i) {
            std::cout << "  " << i + 1 << ": (" << coordinates_[i].first 
                      << ", " << coordinates_[i].second << ")" << std::endl;
        }
    }
};

// Additional event type for demonstration
struct KeyPressEvent {
    char key;
    bool shift_pressed;
};

class Keyboard : public Subject<KeyPressEvent> {
public:
    void pressKey(char key, bool shift = false) {
        std::cout << ">>> Key '" << key << "' pressed" 
                  << (shift ? " (with Shift)" : "") << " <<<" << std::endl;
        KeyPressEvent event{key, shift};
        notify(event);
    }
};

class KeyLogger : public Observer<KeyPressEvent> {
private:
    std::string typed_text_;

public:
    void onEvent(const KeyPressEvent& event) override {
        char key_to_add = event.shift_pressed ? std::toupper(event.key) : event.key;
        typed_text_ += key_to_add;
        std::cout << "[KeyLogger] Added '" << key_to_add 
                  << "' to text. Current: \"" << typed_text_ << "\"" << std::endl;
    }
    
    const std::string& getTypedText() const { return typed_text_; }
};

// Test functions
void testBasicButtonObserver() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST 1: Basic Button Observer Pattern" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    Button button1(1, "OK Button");
    ButtonObserver observer1("BasicObserver");
    
    // Subscribe observer
    button1.subscribe(&observer1);
    
    // Test button clicks
    button1.click(10, 20);
    button1.click(50, 100);
    
    std::cout << "Observer count: " << button1.getObserverCount() << std::endl;
}

void testMultipleObservers() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST 2: Multiple Observers on One Button" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    Button button2(2, "Submit Button");
    
    // Create multiple observers
    ButtonObserver observer1("Observer1");
    ButtonObserver observer2("Observer2");
    ClickCounterObserver counter;
    LoggingObserver logger;
    CoordinateTrackerObserver tracker;
    
    // Subscribe all observers
    button2.subscribe(&observer1);
    button2.subscribe(&observer2);
    button2.subscribe(&counter);
    button2.subscribe(&logger);
    button2.subscribe(&tracker);
    
    std::cout << "Subscribed " << button2.getObserverCount() << " observers" << std::endl;
    
    // Test clicks
    button2.click(25, 75);
    button2.click(100, 150);
    button2.click(200, 300);
    
    // Show final state
    std::cout << "\nFinal click count: " << counter.getClickCount() << std::endl;
    tracker.printAllCoordinates();
}

void testObserverUnsubscription() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST 3: Observer Unsubscription" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    Button button3(3, "Dynamic Button");
    ButtonObserver observer1("PermanentObserver");
    ButtonObserver observer2("TemporaryObserver");
    ClickCounterObserver counter;
    
    // Subscribe observers
    button3.subscribe(&observer1);
    button3.subscribe(&observer2);
    button3.subscribe(&counter);
    
    std::cout << "Initial observer count: " << button3.getObserverCount() << std::endl;
    
    // First click - all observers active
    std::cout << "\nFirst click (all observers active):" << std::endl;
    button3.click(10, 10);
    
    // Unsubscribe temporary observer
    button3.unsubscribe(&observer2);
    std::cout << "\nUnsubscribed TemporaryObserver. Count: " << button3.getObserverCount() << std::endl;
    
    // Second click - temporary observer gone
    std::cout << "\nSecond click (after unsubscription):" << std::endl;
    button3.click(20, 20);
    
    // Show final counter
    std::cout << "\nFinal click count: " << counter.getClickCount() << std::endl;
}

void testMultipleButtonsOneObserver() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST 4: Multiple Buttons, One Observer" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    Button button1(10, "Button A");
    Button button2(20, "Button B");
    Button button3(30, "Button C");
    
    ClickCounterObserver globalCounter;
    LoggingObserver globalLogger;
    
    // Subscribe same observers to multiple buttons
    button1.subscribe(&globalCounter);
    button1.subscribe(&globalLogger);
    
    button2.subscribe(&globalCounter);
    button2.subscribe(&globalLogger);
    
    button3.subscribe(&globalCounter);
    button3.subscribe(&globalLogger);
    
    // Click different buttons
    button1.click(1, 1);
    button2.click(2, 2);
    button1.click(3, 3);
    button3.click(4, 4);
    button2.click(5, 5);
    
    std::cout << "\nTotal clicks across all buttons: " << globalCounter.getClickCount() << std::endl;
}

void testDifferentEventTypes() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST 5: Different Event Types (Keyboard)" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    Keyboard keyboard;
    KeyLogger keyLogger;
    
    keyboard.subscribe(&keyLogger);
    
    // Test key presses
    keyboard.pressKey('h');
    keyboard.pressKey('e');
    keyboard.pressKey('l');
    keyboard.pressKey('l');
    keyboard.pressKey('o');
    keyboard.pressKey(' ');
    keyboard.pressKey('w', true); // Shift+W
    keyboard.pressKey('o');
    keyboard.pressKey('r');
    keyboard.pressKey('l');
    keyboard.pressKey('d', true); // Shift+D
    
    std::cout << "\nFinal typed text: \"" << keyLogger.getTypedText() << "\"" << std::endl;
}

void testEdgeCases() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TEST 6: Edge Cases" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    Button button(99, "Edge Case Button");
    ButtonObserver observer("EdgeObserver");
    
    // Test with no observers
    std::cout << "Clicking with no observers:" << std::endl;
    button.click(0, 0);
    
    // Subscribe observer
    button.subscribe(&observer);
    std::cout << "\nSubscribed observer. Clicking:" << std::endl;
    button.click(1, 1);
    
    // Try to unsubscribe non-existent observer
    ButtonObserver fakeObserver("FakeObserver");
    std::cout << "\nTrying to unsubscribe non-existent observer:" << std::endl;
    std::cout << "Observer count before: " << button.getObserverCount() << std::endl;
    button.unsubscribe(&fakeObserver);
    std::cout << "Observer count after: " << button.getObserverCount() << std::endl;
    
    // Subscribe same observer multiple times
    std::cout << "\nSubscribing same observer again:" << std::endl;
    button.subscribe(&observer);
    std::cout << "Observer count: " << button.getObserverCount() << std::endl;
    
    std::cout << "\nClicking (should see duplicate notifications):" << std::endl;
    button.click(2, 2);
    
    // Unsubscribe all instances
    std::cout << "\nUnsubscribing observer:" << std::endl;
    button.unsubscribe(&observer);
    std::cout << "Observer count: " << button.getObserverCount() << std::endl;
}

int main() {
    std::cout << "OBSERVER PATTERN COMPREHENSIVE TEST SUITE" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    // Run all tests
    testBasicButtonObserver();
    testMultipleObservers();
    testObserverUnsubscription();
    testMultipleButtonsOneObserver();
    testDifferentEventTypes();
    testEdgeCases();

    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "ALL TESTS COMPLETED!" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    return 0;
}