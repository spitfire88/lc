#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <algorithm>
#include <mutex>

// ============================================================================
// EXCEPTION HANDLING - Custom exception classes
// ============================================================================

class CameraException : public std::runtime_error {
public:
    explicit CameraException(const std::string& message) 
        : std::runtime_error("Camera Error: " + message) {}
};

class CameraInitializationException : public CameraException {
public:
    explicit CameraInitializationException(const std::string& message)
        : CameraException("Initialization failed: " + message) {}
};

class CameraOperationException : public CameraException {
public:
    explicit CameraOperationException(const std::string& message)
        : CameraException("Operation failed: " + message) {}
};

// ============================================================================
// TEMPLATES - Image buffer template class with RAII
// ============================================================================

template<typename T>
class ImageBuffer {
private:
    T* data_;
    size_t width_;
    size_t height_;
    size_t channels_;
    size_t size_;

public:
    // Constructor - RAII principle
    ImageBuffer(size_t width, size_t height, size_t channels = 3) 
        : width_(width), height_(height), channels_(channels) {
        size_ = width_ * height_ * channels_;
        data_ = new T[size_];
        std::cout << "ImageBuffer allocated: " << size_ << " elements" << std::endl;
    }

    // Rule of 5: Destructor
    ~ImageBuffer() {
        delete[] data_;
        std::cout << "ImageBuffer deallocated" << std::endl;
    }

    // Rule of 5: Copy constructor
    ImageBuffer(const ImageBuffer& other) 
        : width_(other.width_), height_(other.height_), 
          channels_(other.channels_), size_(other.size_) {
        data_ = new T[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "ImageBuffer copy constructed" << std::endl;
    }

    // Rule of 5: Copy assignment operator
    ImageBuffer& operator=(const ImageBuffer& other) {
        if (this != &other) {
            delete[] data_;
            width_ = other.width_;
            height_ = other.height_;
            channels_ = other.channels_;
            size_ = other.size_;
            data_ = new T[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            std::cout << "ImageBuffer copy assigned" << std::endl;
        }
        return *this;
    }

    // Rule of 5: Move constructor
    ImageBuffer(ImageBuffer&& other) noexcept 
        : data_(other.data_), width_(other.width_), height_(other.height_),
          channels_(other.channels_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "ImageBuffer move constructed" << std::endl;
    }

    // Rule of 5: Move assignment operator
    ImageBuffer& operator=(ImageBuffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            width_ = other.width_;
            height_ = other.height_;
            channels_ = other.channels_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "ImageBuffer move assigned" << std::endl;
        }
        return *this;
    }

    // Accessors
    T* getData() const { return data_; }
    size_t getWidth() const { return width_; }
    size_t getHeight() const { return height_; }
    size_t getChannels() const { return channels_; }
    size_t getSize() const { return size_; }

    // Template method for processing
    template<typename Func>
    void process(Func func) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = func(data_[i]);
        }
    }
};

// ============================================================================
// TEMPLATES - Configuration template
// ============================================================================

template<typename T>
class CameraConfig {
private:
    T width_;
    T height_;
    T fps_;
    std::string format_;

public:
    CameraConfig(T width, T height, T fps, const std::string& format)
        : width_(width), height_(height), fps_(fps), format_(format) {}

    T getWidth() const { return width_; }
    T getHeight() const { return height_; }
    T getFps() const { return fps_; }
    const std::string& getFormat() const { return format_; }

    void setWidth(T width) { width_ = width; }
    void setHeight(T height) { height_ = height; }
    void setFps(T fps) { fps_ = fps; }
    void setFormat(const std::string& format) { format_ = format; }
};

// ============================================================================
// INHERITANCE & VIRTUAL METHODS - Base Camera HAL interface
// ============================================================================

class CameraHAL {
protected:
    std::string camera_id_;
    bool is_initialized_;
    mutable std::mutex mutex_; // For thread safety

public:
    // Constructor
    explicit CameraHAL(const std::string& camera_id) 
        : camera_id_(camera_id), is_initialized_(false) {
        std::cout << "CameraHAL base constructor for camera: " << camera_id_ << std::endl;
    }

    // Virtual destructor - essential for proper cleanup in inheritance
    virtual ~CameraHAL() {
        std::cout << "CameraHAL destructor for camera: " << camera_id_ << std::endl;
        if (is_initialized_) {
            try {
                shutdown();
            } catch (...) {
                // Don't throw from destructor
                std::cerr << "Exception during shutdown in destructor" << std::endl;
            }
        }
    }

    // Pure virtual methods - must be implemented by derived classes
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual void startCapture() = 0;
    virtual void stopCapture() = 0;

    // Virtual methods with default implementation
    virtual void configure(const CameraConfig<int>& config) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Base configuration: " << config.getWidth() << "x" 
                  << config.getHeight() << " @ " << config.getFps() << "fps" << std::endl;
    }

    virtual std::string getStatus() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return is_initialized_ ? "Initialized" : "Not initialized";
    }

    // Template method pattern
    template<typename T>
    ImageBuffer<T> captureFrame(size_t width, size_t height, size_t channels = 3) {
        if (!is_initialized_) {
            throw CameraOperationException("Camera not initialized");
        }
        return ImageBuffer<T>(width, height, channels);
    }

    // Non-virtual public interface
    const std::string& getCameraId() const { return camera_id_; }
    bool isInitialized() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return is_initialized_; 
    }

protected:
    void setInitialized(bool status) {
        std::lock_guard<std::mutex> lock(mutex_);
        is_initialized_ = status;
    }
};

// ============================================================================
// INHERITANCE - USB Camera implementation
// ============================================================================

class USBCamera : public CameraHAL {
private:
    std::string usb_port_;
    int bandwidth_;

public:
    USBCamera(const std::string& camera_id, const std::string& usb_port, int bandwidth = 480)
        : CameraHAL(camera_id), usb_port_(usb_port), bandwidth_(bandwidth) {
        std::cout << "USBCamera constructor for port: " << usb_port_ << std::endl;
    }

    ~USBCamera() override {
        std::cout << "USBCamera destructor" << std::endl;
    }

    // Override pure virtual methods
    void initialize() override {
        std::cout << "Initializing USB camera on port: " << usb_port_ << std::endl;
        
        // Simulate initialization that might fail
        if (usb_port_ == "invalid") {
            throw CameraInitializationException("Invalid USB port: " + usb_port_);
        }
        
        // Simulate hardware initialization delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        setInitialized(true);
        std::cout << "USB camera initialized successfully" << std::endl;
    }

    void shutdown() override {
        std::cout << "Shutting down USB camera" << std::endl;
        setInitialized(false);
    }

    void startCapture() override {
        if (!isInitialized()) {
            throw CameraOperationException("USB camera not initialized");
        }
        std::cout << "Starting USB camera capture" << std::endl;
    }

    void stopCapture() override {
        std::cout << "Stopping USB camera capture" << std::endl;
    }

    // Override virtual method
    void configure(const CameraConfig<int>& config) override {
        CameraHAL::configure(config); // Call base implementation
        std::cout << "USB specific configuration on port: " << usb_port_ 
                  << " with bandwidth: " << bandwidth_ << " Mbps" << std::endl;
    }

    // USB-specific methods
    void setBandwidth(int bandwidth) { 
        bandwidth_ = bandwidth; 
        std::cout << "USB bandwidth set to: " << bandwidth_ << " Mbps" << std::endl;
    }

    const std::string& getUSBPort() const { return usb_port_; }
};

// ============================================================================
// INHERITANCE - Network Camera implementation
// ============================================================================

class NetworkCamera : public CameraHAL {
private:
    std::string ip_address_;
    int port_;
    std::string protocol_;

public:
    NetworkCamera(const std::string& camera_id, const std::string& ip_address, 
                  int port = 554, const std::string& protocol = "RTSP")
        : CameraHAL(camera_id), ip_address_(ip_address), port_(port), protocol_(protocol) {
        std::cout << "NetworkCamera constructor for IP: " << ip_address_ << std::endl;
    }

    ~NetworkCamera() override {
        std::cout << "NetworkCamera destructor" << std::endl;
    }

    // Override pure virtual methods
    void initialize() override {
        std::cout << "Initializing network camera at " << ip_address_ 
                  << ":" << port_ << " using " << protocol_ << std::endl;
        
        // Simulate network connection that might fail
        if (ip_address_ == "0.0.0.0") {
            throw CameraInitializationException("Invalid IP address: " + ip_address_);
        }
        
        // Simulate network connection delay
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        setInitialized(true);
        std::cout << "Network camera connected successfully" << std::endl;
    }

    void shutdown() override {
        std::cout << "Disconnecting network camera" << std::endl;
        setInitialized(false);
    }

    void startCapture() override {
        if (!isInitialized()) {
            throw CameraOperationException("Network camera not connected");
        }
        std::cout << "Starting network stream capture" << std::endl;
    }

    void stopCapture() override {
        std::cout << "Stopping network stream capture" << std::endl;
    }

    // Override virtual method
    std::string getStatus() const override {
        std::string base_status = CameraHAL::getStatus();
        return base_status + " (Network: " + ip_address_ + ":" + std::to_string(port_) + ")";
    }

    // Network-specific methods
    void setCredentials(const std::string& username, const std::string& password) {
        std::cout << "Setting network credentials for user: " << username << std::endl;
    }

    const std::string& getIPAddress() const { return ip_address_; }
    int getPort() const { return port_; }
};

// ============================================================================
// TEMPLATES - Camera Manager with template specialization
// ============================================================================

template<typename CameraType>
class CameraManager {
private:
    std::vector<std::unique_ptr<CameraType>> cameras_;

public:
    // Rule of 0 - using smart pointers, so default operations are sufficient
    CameraManager() = default;
    ~CameraManager() = default;

    // Delete copy operations to follow Rule of 0 properly
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    // Enable move operations
    CameraManager(CameraManager&&) = default;
    CameraManager& operator=(CameraManager&&) = default;

    void addCamera(std::unique_ptr<CameraType> camera) {
        cameras_.push_back(std::move(camera));
    }

    template<typename... Args>
    void createCamera(Args&&... args) {
        cameras_.push_back(std::make_unique<CameraType>(std::forward<Args>(args)...));
    }

    void initializeAll() {
        for (auto& camera : cameras_) {
            try {
                camera->initialize();
            } catch (const CameraException& e) {
                std::cerr << "Failed to initialize camera " << camera->getCameraId() 
                          << ": " << e.what() << std::endl;
                throw; // Re-throw for caller to handle
            }
        }
    }

    void shutdownAll() {
        for (auto& camera : cameras_) {
            try {
                if (camera->isInitialized()) {
                    camera->shutdown();
                }
            } catch (const CameraException& e) {
                std::cerr << "Error during shutdown: " << e.what() << std::endl;
            }
        }
    }

    CameraType* getCamera(const std::string& camera_id) {
        auto it = std::find_if(cameras_.begin(), cameras_.end(),
            [&camera_id](const std::unique_ptr<CameraType>& camera) {
                return camera->getCameraId() == camera_id;
            });
        return (it != cameras_.end()) ? it->get() : nullptr;
    }

    size_t getCameraCount() const { return cameras_.size(); }

    // Template method for applying operations to all cameras
    template<typename Func>
    void forEachCamera(Func func) {
        for (auto& camera : cameras_) {
            func(*camera);
        }
    }
};

// ============================================================================
// DEMONSTRATION FUNCTIONS
// ============================================================================

void demonstrateExceptionHandling() {
    std::cout << "\n=== Exception Handling Demo ===" << std::endl;
    
    try {
        USBCamera invalid_camera("invalid_cam", "invalid");
        invalid_camera.initialize();
    } catch (const CameraInitializationException& e) {
        std::cout << "Caught initialization exception: " << e.what() << std::endl;
    } catch (const CameraException& e) {
        std::cout << "Caught camera exception: " << e.what() << std::endl;
    }

    try {
        NetworkCamera network_cam("net_cam", "0.0.0.0");
        network_cam.initialize();
    } catch (const CameraException& e) {
        std::cout << "Caught network camera exception: " << e.what() << std::endl;
    }
}

void demonstrateTemplates() {
    std::cout << "\n=== Templates Demo ===" << std::endl;
    
    // Template class usage
    ImageBuffer<uint8_t> rgb_buffer(1920, 1080, 3);
    ImageBuffer<float> hdr_buffer(1920, 1080, 4);
    
    // Template method usage
    rgb_buffer.process([](uint8_t pixel) -> uint8_t {
        return std::min(255, static_cast<int>(pixel * 1.2)); // Brightness adjustment
    });
    
    // Configuration template
    CameraConfig<int> config_hd(1920, 1080, 30, "RGB");
    CameraConfig<float> config_scientific(2048.5, 2048.5, 15.5, "RAW");
    
    std::cout << "HD Config: " << config_hd.getWidth() << "x" << config_hd.getHeight() << std::endl;
    std::cout << "Scientific Config: " << config_scientific.getWidth() << "x" 
              << config_scientific.getHeight() << std::endl;
}

void demonstratePolymorphism() {
    std::cout << "\n=== Polymorphism Demo ===" << std::endl;
    
    std::vector<std::unique_ptr<CameraHAL>> cameras;
    cameras.push_back(std::make_unique<USBCamera>("usb_cam_1", "/dev/video0", 480));
    cameras.push_back(std::make_unique<NetworkCamera>("net_cam_1", "192.168.1.100", 554));
    
    CameraConfig<int> config(1280, 720, 30, "YUV");
    
    for (auto& camera : cameras) {
        try {
            std::cout << "\nCamera: " << camera->getCameraId() << std::endl;
            camera->initialize();
            camera->configure(config);
            std::cout << "Status: " << camera->getStatus() << std::endl;
            camera->startCapture();
            
            // Demonstrate template method
            auto frame = camera->captureFrame<uint8_t>(640, 480);
            std::cout << "Captured frame: " << frame.getWidth() << "x" 
                      << frame.getHeight() << std::endl;
            
            camera->stopCapture();
        } catch (const CameraException& e) {
            std::cout << "Camera operation failed: " << e.what() << std::endl;
        }
    }
}

void demonstrateCameraManager() {
    std::cout << "\n=== Camera Manager Demo ===" << std::endl;
    
    try {
        CameraManager<USBCamera> usb_manager;
        usb_manager.createCamera("usb_cam_2", "/dev/video1", 1000);
        usb_manager.createCamera("usb_cam_3", "/dev/video2", 1000);
        
        CameraManager<NetworkCamera> network_manager;
        network_manager.createCamera("net_cam_2", "192.168.1.101", 8080, "HTTP");
        
        std::cout << "USB Manager has " << usb_manager.getCameraCount() << " cameras" << std::endl;
        std::cout << "Network Manager has " << network_manager.getCameraCount() << " cameras" << std::endl;
        
        // Initialize all USB cameras
        usb_manager.initializeAll();
        
        // Apply operation to all cameras using template method
        usb_manager.forEachCamera([](USBCamera& camera) {
            camera.setBandwidth(2000);
            std::cout << "Set bandwidth for camera: " << camera.getCameraId() << std::endl;
        });
        
        // Cleanup
        usb_manager.shutdownAll();
        
    } catch (const CameraException& e) {
        std::cout << "Manager operation failed: " << e.what() << std::endl;
    }
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    std::cout << "=== Camera HAL C++ Features Demonstration ===" << std::endl;
    
    try {
        // Demonstrate exception handling
        demonstrateExceptionHandling();
        
        // Demonstrate templates
        demonstrateTemplates();
        
        // Demonstrate inheritance, virtual methods, and polymorphism
        demonstratePolymorphism();
        
        // Demonstrate template manager and RAII
        demonstrateCameraManager();
        
        std::cout << "\n=== RAII Demo (automatic cleanup) ===" << std::endl;
        {
            USBCamera scoped_camera("scoped_cam", "/dev/video3");
            scoped_camera.initialize();
            std::cout << "Scoped camera created and initialized" << std::endl;
            // Automatic cleanup when going out of scope
        }
        std::cout << "Scoped camera automatically destroyed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Program completed successfully ===" << std::endl;
    return 0;
}
