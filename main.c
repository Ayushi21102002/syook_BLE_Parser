#include <stdio.h>
#include <unistd.h> // For sleep function
#include <stdint.h>
#include <math.h>

// BLEDeviceData structure definition
typedef struct {
    uint8_t data_len;
    uint8_t flag_type;
    uint8_t flag;
    uint8_t service_len;
    uint8_t service_id;
    uint16_t uuid;
    uint8_t service_len_2;
    uint8_t service_type;
    uint16_t uuid_2;
    uint8_t frame;
    uint8_t version;
    uint8_t battery;
    float x;
    float y;
    float z;
    char mac[13];
} BLEDeviceData;

// Function to parse BLE device data
void parse_device_data(const char *data, BLEDeviceData *device_data) {
    // Convert hex string to byte array
    uint8_t bytes[26];
    for (int i = 0; i < 26; i++) {
        unsigned int temp;
        sscanf(data + 2 * i, "%2x", &temp);
        bytes[i] = (uint8_t)temp;
    }

    // Parse fields based on the given structure
    device_data->data_len = bytes[0];
    device_data->flag_type = bytes[1];
    device_data->flag = bytes[2];
    device_data->service_len = bytes[3];
    device_data->service_id = bytes[4];
    device_data->uuid = (uint16_t)((uint16_t)bytes[5] << 8 | bytes[6]);
    device_data->service_len_2 = bytes[7];
    device_data->service_type = bytes[8];
    device_data->uuid_2 = (uint16_t)((uint16_t)bytes[9] << 8 | bytes[10]);
    device_data->frame = bytes[11];
    device_data->version = bytes[12];
    device_data->battery = bytes[13];
    device_data->x = (int16_t)((int16_t)bytes[14] << 8 | bytes[15]) / 256.0;
    device_data->y = (int16_t)((int16_t)bytes[16] << 8 | bytes[17]) / 256.0;
    device_data->z = (int16_t)((int16_t)bytes[18] << 8 | bytes[19]) / 256.0;
    snprintf(device_data->mac, sizeof(device_data->mac), "%02X%02X%02X%02X%02X%02X",
             bytes[20], bytes[21], bytes[22], bytes[23], bytes[24], bytes[25]);
}

// Function to print parsed device data
void print_device_data(const BLEDeviceData *data) {
    printf("Device Information:\n");
    printf("  MAC Address: %s\n", data->mac);
    printf("  UUID: %04X\n", data->uuid);
    printf("  UUID 2: %04X\n", data->uuid_2);
    printf("  Frame: %u\n", data->frame);
    printf("  Version: %u\n", data->version);
    printf("  Battery: %u%%\n", data->battery);

    printf("\nSensor Data:\n");
    printf("  X: %.2fg\n", data->x);
    printf("  Y: %.2fg\n", data->y);
    printf("  Z: %.2fg\n", data->z);

    printf("\nRaw Data:\n");
    printf("  Data Length: %u\n", data->data_len);
    printf("  Flag Type: %u\n", data->flag_type);
    printf("  Flag: %u\n", data->flag);
    printf("  Service Length: %u\n", data->service_len);
    printf("  Service ID: %u\n", data->service_id);
    printf("  Service Length 2: %u\n", data->service_len_2);
    printf("  Service Type: %u\n", data->service_type);
}

// Function to check if the device is moving
int device_moving_fun(const BLEDeviceData *device_data) {
    double magnitude = sqrt(pow(device_data->x, 2) +
                            pow(device_data->y, 2) +
                            pow(device_data->z, 2));

    const double threshold = 1.0;
    printf("Magnitude: %f\n", magnitude);

    return magnitude > threshold;
}

int main() {
    // Example data samples from the BLE device
    const char *packets[] = {
        "0201060303E1FF1216E1FFA103640000FF7F00C23772A33F23AC",
        "0201060303E1FF1216E1FFA1036400FAFFE01233772A33F23AC",
        "0201060303E1FF1216E1FFA1036400FBFF7D013F3772A33F23AC",
        "0201060303E1FF1216E1FFA103640000FF00000F3772A33F23AC",
        "0201060303E1FF1216E1FFA1036400FDFF0B013F3772A33F23AC",
        "0201060303E1FF1216E1FFA1036400000000000F3772A33F23AC"
    };

    for (int i = 0; i < 6; i++) {
        BLEDeviceData device_data;
        parse_device_data(packets[i], &device_data);
        print_device_data(&device_data);

        if (device_moving_fun(&device_data)) {
            printf("\nThe device is moving.\n");
        } else {
            printf("\nThe device is not moving.\n");
        }

        // Sleep for 1 second to simulate real-time data ingestion
        sleep(1);
    }

    return 0;
}