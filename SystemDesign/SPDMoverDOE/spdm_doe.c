#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations
typedef struct DOELayer DOELayer;
typedef struct SPDMLayer SPDMLayer;

// SPDM Message Types
typedef enum {
    SPDM_GET_VERSION = 0x84,
    SPDM_VERSION = 0x04,
    SPDM_GET_CAPABILITIES = 0x81,
    SPDM_CAPABILITIES = 0x61,
    SPDM_NEGOTIATE_ALGORITHMS = 0x63,
    SPDM_ALGORITHMS = 0x63,
    SPDM_GET_DIGESTS = 0x81,
    SPDM_DIGESTS = 0x01
} SPDMMessageType;

// DOE Protocol Types
typedef enum {
    DOE_PROTOCOL_SPDM = 0x0001,
    DOE_PROTOCOL_CMA = 0x0002,
    DOE_PROTOCOL_SECURED_SPDM = 0x0100
} DOEProtocolType;

// DOE Discovery Response
typedef struct {
    uint16_t vendor_id;
    uint8_t data_object_type;
    uint8_t version;
} DOEDiscoveryResponse;

// Message structure
#define MAX_MESSAGE_SIZE 1024
typedef struct {
    uint8_t data[MAX_MESSAGE_SIZE];
    size_t size;
} Message;

// Function pointer type for SPDM message handlers
typedef Message (*SPDMMessageHandler)(SPDMLayer* spdm, const Message* msg);

// SPDM Layer structure
typedef struct SPDMLayer {
    DOELayer* doe_layer;
    SPDMMessageHandler handlers[256]; // Indexed by message type
} SPDMLayer;

// DOE Layer structure
#define MAX_DISCOVERED_PROTOCOLS 10
typedef struct DOELayer {
    SPDMLayer* spdm_handler;
    DOEDiscoveryResponse discovered_protocols[MAX_DISCOVERED_PROTOCOLS];
    size_t num_discovered;
} DOELayer;

// Message helper functions
void message_init(Message* msg) {
    memset(msg->data, 0, MAX_MESSAGE_SIZE);
    msg->size = 0;
}

void message_append_byte(Message* msg, uint8_t byte) {
    if (msg->size < MAX_MESSAGE_SIZE) {
        msg->data[msg->size++] = byte;
    }
}

void message_append_data(Message* msg, const uint8_t* data, size_t len) {
    size_t copy_len = (msg->size + len > MAX_MESSAGE_SIZE) ? 
                      MAX_MESSAGE_SIZE - msg->size : len;
    memcpy(&msg->data[msg->size], data, copy_len);
    msg->size += copy_len;
}

// SPDM Layer function declarations
Message spdm_handle_get_version(SPDMLayer* spdm, const Message* msg);
Message spdm_handle_get_capabilities(SPDMLayer* spdm, const Message* msg);
Message spdm_send_via_doe(SPDMLayer* spdm, const Message* msg);
Message spdm_create_version_request(void);
bool spdm_process_version_response(const Message* response);
bool spdm_negotiate_capabilities(void);
bool spdm_initialize_session(SPDMLayer* spdm);
Message spdm_process_message(SPDMLayer* spdm, const Message* msg);

// DOE Layer function declarations
void doe_perform_discovery(DOELayer* doe);
void doe_register_spdm_layer(DOELayer* doe, SPDMLayer* spdm);
Message doe_send_spdm_message(DOELayer* doe, const Message* spdm_msg);
void doe_process_incoming_message(DOELayer* doe, const Message* msg);
Message doe_wrap_in_header(const Message* payload, DOEProtocolType protocol);
Message doe_extract_spdm_message(const Message* doe_msg);
Message doe_simulate_transport(const Message* msg);

// SPDM Layer implementation
SPDMLayer* spdm_create(DOELayer* doe) {
    SPDMLayer* spdm = malloc(sizeof(SPDMLayer));
    if (!spdm) return NULL;
    
    spdm->doe_layer = doe;
    
    // Initialize all handlers to NULL
    memset(spdm->handlers, 0, sizeof(spdm->handlers));
    
    // Setup message handlers
    spdm->handlers[SPDM_GET_VERSION] = spdm_handle_get_version;
    spdm->handlers[SPDM_GET_CAPABILITIES] = spdm_handle_get_capabilities;
    
    return spdm;
}

void spdm_destroy(SPDMLayer* spdm) {
    if (spdm) {
        free(spdm);
    }
}

bool spdm_initialize_session(SPDMLayer* spdm) {
    printf("SPDM: Initializing session\n");
    
    // Send GET_VERSION via DOE
    Message version_req = spdm_create_version_request();
    Message response = spdm_send_via_doe(spdm, &version_req);
    
    if (spdm_process_version_response(&response)) {
        printf("SPDM: Version negotiation successful\n");
        return spdm_negotiate_capabilities();
    }
    return false;
}

Message spdm_send_via_doe(SPDMLayer* spdm, const Message* msg) {
    Message empty_msg;
    message_init(&empty_msg);
    
    if (spdm->doe_layer) {
        return doe_send_spdm_message(spdm->doe_layer, msg);
    }
    return empty_msg;
}

Message spdm_process_message(SPDMLayer* spdm, const Message* msg) {
    Message empty_msg;
    message_init(&empty_msg);
    
    if (msg->size == 0) return empty_msg;
    
    SPDMMessageType type = (SPDMMessageType)msg->data[0];
    SPDMMessageHandler handler = spdm->handlers[type];
    
    if (handler) {
        return handler(spdm, msg);
    }
    
    printf("SPDM: Unknown message type: 0x%02X\n", type);
    return empty_msg;
}

Message spdm_create_version_request(void) {
    Message msg;
    message_init(&msg);
    message_append_byte(&msg, SPDM_GET_VERSION);
    message_append_byte(&msg, 0x10); // Version 1.0
    printf("SPDM: Created GET_VERSION request\n");
    return msg;
}

bool spdm_process_version_response(const Message* response) {
    if (response->size > 0 && response->data[0] == SPDM_VERSION) {
        printf("SPDM: Received valid VERSION response\n");
        return true;
    }
    return false;
}

bool spdm_negotiate_capabilities(void) {
    printf("SPDM: Negotiating capabilities\n");
    // Simplified capability negotiation
    return true;
}

Message spdm_handle_get_version(SPDMLayer* spdm, const Message* msg) {
    printf("SPDM: Handling GET_VERSION\n");
    Message response;
    message_init(&response);
    message_append_byte(&response, SPDM_VERSION);
    message_append_byte(&response, 0x10); // Support v1.0
    message_append_byte(&response, 0x11); // Support v1.1
    return response;
}

Message spdm_handle_get_capabilities(SPDMLayer* spdm, const Message* msg) {
    printf("SPDM: Handling GET_CAPABILITIES\n");
    Message response;
    message_init(&response);
    message_append_byte(&response, SPDM_CAPABILITIES);
    message_append_byte(&response, 0x01); // Sample capabilities
    message_append_byte(&response, 0x02);
    return response;
}

// DOE Layer implementation
DOELayer* doe_create(void) {
    DOELayer* doe = malloc(sizeof(DOELayer));
    if (!doe) return NULL;
    
    doe->spdm_handler = NULL;
    doe->num_discovered = 0;
    
    doe_perform_discovery(doe);
    return doe;
}

void doe_destroy(DOELayer* doe) {
    if (doe) {
        free(doe);
    }
}

void doe_register_spdm_layer(DOELayer* doe, SPDMLayer* spdm) {
    doe->spdm_handler = spdm;
    printf("DOE: SPDM layer registered\n");
}

Message doe_send_spdm_message(DOELayer* doe, const Message* spdm_msg) {
    printf("DOE: Sending SPDM message of size %zu\n", spdm_msg->size);
    
    // Wrap SPDM message in DOE header
    Message doe_msg = doe_wrap_in_header(spdm_msg, DOE_PROTOCOL_SPDM);
    
    // Simulate sending to hardware/transport layer
    Message response = doe_simulate_transport(&doe_msg);
    
    // Extract SPDM response from DOE wrapper
    return doe_extract_spdm_message(&response);
}

void doe_process_incoming_message(DOELayer* doe, const Message* msg) {
    if (msg->size < 4) return;
    
    // Extract DOE protocol type from header (little endian)
    DOEProtocolType protocol = (DOEProtocolType)((msg->data[3] << 8) | msg->data[2]);
    
    printf("DOE: Processing message for protocol type 0x%04X\n", protocol);
    
    if (protocol == DOE_PROTOCOL_SPDM && doe->spdm_handler) {
        Message spdm_msg = doe_extract_spdm_message(msg);
        Message response = spdm_process_message(doe->spdm_handler, &spdm_msg);
        
        if (response.size > 0) {
            Message doe_response = doe_wrap_in_header(&response, protocol);
            // Send response back through transport
            doe_simulate_transport(&doe_response);
        }
    }
}

void doe_perform_discovery(DOELayer* doe) {
    printf("DOE: Performing protocol discovery\n");
    
    // Simulate discovery of available protocols
    if (doe->num_discovered < MAX_DISCOVERED_PROTOCOLS) {
        doe->discovered_protocols[doe->num_discovered++] = (DOEDiscoveryResponse){
            .vendor_id = 0x1234,
            .data_object_type = DOE_PROTOCOL_SPDM,
            .version = 0x10
        };
    }
    
    if (doe->num_discovered < MAX_DISCOVERED_PROTOCOLS) {
        doe->discovered_protocols[doe->num_discovered++] = (DOEDiscoveryResponse){
            .vendor_id = 0x1234,
            .data_object_type = DOE_PROTOCOL_CMA,
            .version = 0x20
        };
    }
    
    printf("DOE: Discovered %zu protocols\n", doe->num_discovered);
}

Message doe_wrap_in_header(const Message* payload, DOEProtocolType protocol) {
    Message wrapped;
    message_init(&wrapped);
    
    // DOE Header: [Length][Reserved][Protocol Type][Payload...]
    uint16_t total_length = payload->size + 4;
    message_append_byte(&wrapped, total_length & 0xFF);        // Length low
    message_append_byte(&wrapped, (total_length >> 8) & 0xFF); // Length high
    message_append_byte(&wrapped, protocol & 0xFF);            // Protocol low
    message_append_byte(&wrapped, (protocol >> 8) & 0xFF);     // Protocol high
    
    // Add payload
    message_append_data(&wrapped, payload->data, payload->size);
    
    return wrapped;
}

Message doe_extract_spdm_message(const Message* doe_msg) {
    Message spdm_msg;
    message_init(&spdm_msg);
    
    if (doe_msg->size > 4) {
        size_t payload_size = doe_msg->size - 4;
        message_append_data(&spdm_msg, &doe_msg->data[4], payload_size);
    }
    
    return spdm_msg;
}

Message doe_simulate_transport(const Message* msg) {
    printf("DOE: Simulating transport layer (size: %zu)\n", msg->size);
    
    Message response;
    message_init(&response);
    
    // Simulate echo response for testing
    if (msg->size > 0) {
        message_append_data(&response, msg->data, msg->size);
        // Modify first payload byte to simulate different response
        if (response.size > 4) {
            response.data[4] = response.data[4] | 0x80; // Convert request to response
        }
    }
    
    return response;
}

// Demo application
typedef struct {
    DOELayer* doe;
    SPDMLayer* spdm;
} SPDMApplication;

SPDMApplication* app_create(void) {
    SPDMApplication* app = malloc(sizeof(SPDMApplication));
    if (!app) return NULL;
    
    // Create layers
    app->doe = doe_create();
    if (!app->doe) {
        free(app);
        return NULL;
    }
    
    app->spdm = spdm_create(app->doe);
    if (!app->spdm) {
        doe_destroy(app->doe);
        free(app);
        return NULL;
    }
    
    // Register SPDM with DOE
    doe_register_spdm_layer(app->doe, app->spdm);
    
    return app;
}

void app_destroy(SPDMApplication* app) {
    if (app) {
        spdm_destroy(app->spdm);
        doe_destroy(app->doe);
        free(app);
    }
}

void app_run(SPDMApplication* app) {
    printf("\n=== SPDM/DOE Layer Demo ===\n");
    
    // Initialize SPDM session
    if (spdm_initialize_session(app->spdm)) {
        printf("\nSPDM session established successfully!\n");
        
        // Simulate incoming DOE message
        printf("\n--- Simulating incoming DOE message ---\n");
        Message incoming;
        message_init(&incoming);
        
        // DOE header + SPDM GET_VERSION
        uint8_t incoming_data[] = {0x08, 0x00, 0x01, 0x00, 0x84, 0x10};
        message_append_data(&incoming, incoming_data, sizeof(incoming_data));
        
        doe_process_incoming_message(app->doe, &incoming);
    } else {
        printf("\nFailed to establish SPDM session\n");
    }
}

int main(void) {
    SPDMApplication* app = app_create();
    if (!app) {
        printf("Failed to create application\n");
        return 1;
    }
    
    app_run(app);
    app_destroy(app);
    
    return 0;
}