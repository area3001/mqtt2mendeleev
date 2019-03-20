#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <locale.h>

#include <mendeleev/mendeleev.h>
#include <mosquitto.h>

#define NAME "mqtt2mendeleev"
#ifndef VERSION
#define VERSION "<undefined version>"
#endif

#define MAX_DATA_LENGTH 240
#define MQTT_PREFIX "mendeleev/"

enum PeriodicElement {
    ELEMENT_H   =   1, // Hydrogen
    ELEMENT_He  =   2, // Helium
    ELEMENT_Li  =   3, // Lithium
    ELEMENT_Be  =   4, // Beryllium
    ELEMENT_B   =   5, // Boron
    ELEMENT_C   =   6, // Carbon
    ELEMENT_N   =   7, // Nitrogen
    ELEMENT_O   =   8, // Oxygen
    ELEMENT_F   =   9, // Fluorine
    ELEMENT_Ne  =  10, // Neon
    ELEMENT_Na  =  11, // Sodium
    ELEMENT_Mg  =  12, // Magnesium
    ELEMENT_Al  =  13, // Aluminum
    ELEMENT_Si  =  14, // Silicon
    ELEMENT_P   =  15, // Phosphorus
    ELEMENT_S   =  16, // Sulfur
    ELEMENT_Cl  =  17, // Chlorine
    ELEMENT_Ar  =  18, // Argon
    ELEMENT_K   =  19, // Potassium
    ELEMENT_Ca  =  20, // Calcium
    ELEMENT_Sc  =  21, // Scandium
    ELEMENT_Ti  =  22, // Titanium
    ELEMENT_V   =  23, // Vanadium
    ELEMENT_Cr  =  24, // Chromium
    ELEMENT_Mn  =  25, // Manganese
    ELEMENT_Fe  =  26, // Iron
    ELEMENT_Co  =  27, // Cobalt
    ELEMENT_Ni  =  28, // Nickel
    ELEMENT_Cu  =  29, // Copper
    ELEMENT_Zn  =  30, // Zinc
    ELEMENT_Ga  =  31, // Gallium
    ELEMENT_Ge  =  32, // Germanium
    ELEMENT_As  =  33, // Arsenic
    ELEMENT_Se  =  34, // Selenium
    ELEMENT_Br  =  35, // Bromine
    ELEMENT_Kr  =  36, // Krypton
    ELEMENT_Rb  =  37, // Rubidium
    ELEMENT_Sr  =  38, // Strontium
    ELEMENT_Y   =  39, // Yttrium
    ELEMENT_Zr  =  40, // Zirconium
    ELEMENT_Nb  =  41, // Niobium
    ELEMENT_Mo  =  42, // Molybdenum
    ELEMENT_Tc  =  43, // Technetium
    ELEMENT_Ru  =  44, // Ruthenium
    ELEMENT_Rh  =  45, // Rhodium
    ELEMENT_Pd  =  46, // Palladium
    ELEMENT_Ag  =  47, // Silver
    ELEMENT_Cd  =  48, // Cadmium
    ELEMENT_In  =  49, // Indium
    ELEMENT_Sn  =  50, // Tin
    ELEMENT_Sb  =  51, // Antimony
    ELEMENT_Te  =  52, // Tellurium
    ELEMENT_I   =  53, // Iodine
    ELEMENT_Xe  =  54, // Xenon
    ELEMENT_Cs  =  55, // Cesium
    ELEMENT_Ba  =  56, // Barium
    ELEMENT_La  =  57, // Lanthanum
    ELEMENT_Ce  =  58, // Cerium
    ELEMENT_Pr  =  59, // Praseodymium
    ELEMENT_Nd  =  60, // Neodymium
    ELEMENT_Pm  =  61, // Promethium
    ELEMENT_Sm  =  62, // Samarium
    ELEMENT_Eu  =  63, // Europium
    ELEMENT_Gd  =  64, // Gadolinium
    ELEMENT_Tb  =  65, // Terbium
    ELEMENT_Dy  =  66, // Dysprosium
    ELEMENT_Ho  =  67, // Holmium
    ELEMENT_Er  =  68, // Erbium
    ELEMENT_Tm  =  69, // Thulium
    ELEMENT_Yb  =  70, // Ytterbium
    ELEMENT_Lu  =  71, // Lutetium
    ELEMENT_Hf  =  72, // Hafnium
    ELEMENT_Ta  =  73, // Tantalum
    ELEMENT_W   =  74, // Tungsten
    ELEMENT_Re  =  75, // Rhenium
    ELEMENT_Os  =  76, // Osmium
    ELEMENT_Ir  =  77, // Iridium
    ELEMENT_Pt  =  78, // Platinum
    ELEMENT_Au  =  79, // Gold
    ELEMENT_Hg  =  80, // Mercury
    ELEMENT_Tl  =  81, // Thallium
    ELEMENT_Pb  =  82, // Lead
    ELEMENT_Bi  =  83, // Bismuth
    ELEMENT_Po  =  84, // Polonium
    ELEMENT_At  =  85, // Astatine
    ELEMENT_Rn  =  86, // Radon
    ELEMENT_Fr  =  87, // Francium
    ELEMENT_Ra  =  88, // Radium
    ELEMENT_Ac  =  89, // Actinium
    ELEMENT_Th  =  90, // Thorium
    ELEMENT_Pa  =  91, // Protactinium
    ELEMENT_U   =  92, // Uranium
    ELEMENT_Np  =  93, // Neptunium
    ELEMENT_Pu  =  94, // Plutonium
    ELEMENT_Am  =  95, // Americium
    ELEMENT_Cm  =  96, // Curium
    ELEMENT_Bk  =  97, // Berkelium
    ELEMENT_Cf  =  98, // Californium
    ELEMENT_Es  =  99, // Einsteinium
    ELEMENT_Fm  = 100, // Fermium
    ELEMENT_Md  = 101, // Mendelevium
    ELEMENT_No  = 102, // Nobelium
    ELEMENT_Lr  = 103, // Lawrencium
    ELEMENT_Rf  = 104, // Rutherfordium
    ELEMENT_Db  = 105, // Dubnium
    ELEMENT_Sg  = 106, // Seaborgium
    ELEMENT_Bh  = 107, // Bohrium
    ELEMENT_Hs  = 108, // Hassium
    ELEMENT_Mt  = 109, // Meitnerium
    ELEMENT_Ds  = 110, // Darmstadtium
    ELEMENT_Rg  = 111, // Roentgenium
    ELEMENT_Uub = 112, // Ununbium
    ELEMENT_Uut = 113, // Ununtrium
    ELEMENT_Uuq = 114, // Ununquadium
    ELEMENT_Uup = 115, // Ununpentium
    ELEMENT_Uuh = 116, // Ununhexium
    ELEMENT_Uus = 117, // Ununseptium
    ELEMENT_Uuo = 118, // Oganesson
    ELEMENT_MAX = 119
};

mendeleev_t *mb = NULL;

struct mosquitto *mosq = NULL;

/* MQTT parameters */
static const char *mqtt_host = "localhost";
static int mqtt_port = 1883;
static int mqtt_keepalive = 10;
static int mqtt_qos = -1;

/* tty */
static const char *ttydev;

/* program options */
static const char help_msg[] =
    NAME ": Send mendeleev messages via MQTT\n"
    "usage:	" NAME " [OPTIONS ...] [DEVICE]\n"
    "\n"
    "Options\n"
    " -V, --version		Show version\n"
    "\n"
    " -h, --host=HOST[:PORT]Specify alternate MQTT host+port\n"
    "\n"
    "Arguments\n"
    " DEVICE	use DEVICE to communicate\n"
    ;

#ifdef _GNU_SOURCE
static struct option long_opts[] = {
    { "help", no_argument, NULL, '?', },
    { "version", no_argument, NULL, 'V', },

    { "host", required_argument, NULL, 'h', },
    { },
};
#else
#define getopt_long(argc, argv, optstring, longopts, longindex) \
    getopt((argc), (argv), (optstring))
#endif
static const char optstring[] = "Vv?h:";

void mb_connect(const char *device)
{
    mb = mendeleev_new_rtu(device, 38400, 'N', 8, 1);

    if (mb == NULL) {
        fprintf(stderr, "mendeleev_new_rtu: Call failed\n");
        exit(EXIT_FAILURE);
    }

    if (mendeleev_set_debug(mb, true) == -1) {
        fprintf(stderr, "mendeleev_set_debug: %s\n", mendeleev_strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (mendeleev_connect(mb) == -1) {
        fprintf(stderr, "mendeleev_connect: %s\n", mendeleev_strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int mb_set_color(uint8_t *colors)
{
    if (mendeleev_send_command(mb, MENDELEEV_CMD_SET_COLOR, colors, sizeof(uint8_t) * 7, NULL, NULL) == -1) {
        fprintf(stderr, "mendeleev_set_color: %s\n", mendeleev_strerror(errno));
        return -1;
    }
    return 0;
}

int mb_set_output(uint16_t values)
{
    if (mendeleev_send_command(mb, MENDELEEV_CMD_SET_OUTPUT, (uint8_t *)(&values), sizeof(values), NULL, NULL) == -1) {
        fprintf(stderr, "mendeleev_set_output: %s\n", mendeleev_strerror(errno));
        return -1;
    }
    return 0;
}

int mb_set_mode(uint8_t mode)
{
    if (mendeleev_send_command(mb, MENDELEEV_CMD_SET_MODE, &mode, sizeof(mode), NULL, NULL) == -1) {
        fprintf(stderr, "mendeleev_set_mode: %s\n", mendeleev_strerror(errno));
        return -1;
    }
    return 0;
}

int mb_ota(uint8_t *data, int datalength)
{
    if (mendeleev_send_command(mb, MENDELEEV_CMD_OTA, data, datalength, NULL, NULL) == -1) {
        fprintf(stderr, "mendeleev_ota: %s\n", mendeleev_strerror(errno));
        return -1;
    }
    return 0;
}

int mb_get_version(uint8_t *data, uint16_t *datalength)
{
    if (mendeleev_send_command(mb, MENDELEEV_CMD_GET_VERSION, NULL, 0, data, datalength) == -1)
    {
        fprintf(stderr, "mendeleev_get_version: %s\n", mendeleev_strerror(errno));
        return -1;
    }
    return 0;
}

/*
 * Subscribe to MQTT_PREFIX/+/+
 */
void mqtt_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    if (rc == 0) {
        char *topic = malloc(strlen(MQTT_PREFIX) + 3 + 1);

        if (topic == NULL) {
            fprintf(stderr, "malloc: Not enough memory\n");
            exit(EXIT_FAILURE);
        }

        strcpy(topic, MQTT_PREFIX);
        strcat(topic, "+/+");

        if (mosquitto_subscribe(mosq, NULL, topic, 1) != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "mosquitto_subscribe: Call failed\n");
        }

        free(topic);
    }
    else {
        fprintf(stderr, "mqtt_connect_callback: Connection failed\n");
    }
}

void mqtt_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    int err = 0;
    char strid[4];
    uint8_t data[MAX_DATA_LENGTH];
    uint8_t response[MAX_DATA_LENGTH];
    uint16_t response_length = 0;
    char *id_start = strchr(message->topic, '/');
    if (id_start == NULL) {
        fprintf(stderr, "Could not find /\n");
        return;
    }
    char *id_stop = strchr(id_start+1, '/');
    if (id_stop == NULL) {
        fprintf(stderr, "Could not find second /\n");
        return;
    }
    if ((id_stop-id_start-1) > sizeof(strid)) {
        fprintf(stderr, "Id too long\n");
        return;
    }

    strncpy(strid, id_start+1, id_stop-id_start-1);
    uint8_t id = (uint8_t)atoi(strid);

    if (id == 0 || (id >= ELEMENT_MAX && id < MENDELEEV_BROADCAST_ADDRESS)) {
        fprintf(stderr, "id not valid %d\n", id);
        return;
    }

    if (mendeleev_set_slave(mb, id) == -1) {
        fprintf(stderr, "mendeleev_set_slave with id %d: %s\n", id, mendeleev_strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (strcmp(id_stop+1, "setcolor") == 0) {
        uint8_t *colors;
        if (message->payloadlen == (sizeof(uint8_t) * 7)) {
            colors = (uint8_t *)(message->payload);
            err = mb_set_color(colors);
        }
        else {
            fprintf(stderr, "setcolor failed: wrong payload\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(id_stop+1, "setoutput") == 0) {
        uint16_t values;
        if (message->payloadlen == sizeof(values)) {
            values = *((uint16_t *)(message->payload));
            err = mb_set_output(values);
        }
        else {
            fprintf(stderr, "setoutput failed: wrong payload\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(id_stop+1, "setmode") == 0) {
        uint8_t mode;
        if (message->payloadlen == sizeof(mode)) {
            mode = *((uint8_t *)(message->payload));
            err = mb_set_mode(mode);
        }
        else {
            fprintf(stderr, "setcolor failed: wrong payload\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(id_stop+1, "ota") == 0) {
        int i = 0;
        while (i < message->payloadlen && !err) {
            uint16_t remaining = message->payloadlen - i;
            data[0] = remaining >> 8;
            data[1] = remaining & 0x00FF;
            if (remaining < (MAX_DATA_LENGTH - sizeof(remaining))) {
                memcpy(data + sizeof(remaining), (uint8_t *)message->payload + i, remaining);
                err = mb_ota(data, remaining + sizeof(remaining));
                break;
            }
            else {
                memcpy(data + sizeof(remaining), (uint8_t *)message->payload + i, (MAX_DATA_LENGTH - sizeof(remaining)));
                err = mb_ota(data, MAX_DATA_LENGTH);
                i += (MAX_DATA_LENGTH - sizeof(remaining));
            }
        }
    }
    else if (strcmp(id_stop+1, "version") == 0) {
        err = mb_get_version(response, &response_length);
    }
    else {
        fprintf(stderr, "Unknown command \"%s\"\n", id_stop);
        exit(EXIT_FAILURE);
    }

    if (!err) {
        char *responsetopic = malloc(strlen(message->topic) + 4 + 1);

        if (responsetopic == NULL) {
            fprintf(stderr, "malloc: Not enough memory\n");
            exit(EXIT_FAILURE);
        }

        strcpy(responsetopic, message->topic);
        strcat(responsetopic, "/ack");

        if (mosquitto_publish(mosq, NULL, responsetopic, response_length, response, 1, false) != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "mosquitto_publish: Call failed\n");
        }

        free(responsetopic);
    }
    else {
        char *responsetopic = malloc(strlen(message->topic) + 5 + 1);

        if (responsetopic == NULL) {
            fprintf(stderr, "malloc: Not enough memory\n");
            exit(EXIT_FAILURE);
        }

        strcpy(responsetopic, message->topic);
        strcat(responsetopic, "/nack");

        if (mosquitto_publish(mosq, NULL, responsetopic, response_length, response, 1, false) != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "mosquitto_publish: Call failed\n");
        }

        free(responsetopic);
    }
}

void cleanup(void)
{
    if (mosq != NULL)
    {
        mosquitto_destroy(mosq);
    }

    mosquitto_lib_cleanup();

    if (mb != NULL)
    {
        mendeleev_close(mb);
        mendeleev_free(mb);
    }
}

int main(int argc, char **argv)
{
    int opt;
    char *str;
    char mqtt_name[32];

    setlocale(LC_ALL, "");
    /* argument parsing */
    while ((opt = getopt_long(argc, argv, optstring, long_opts, NULL)) >= 0)
    switch (opt) {
    case 'V':
        fprintf(stderr, "%s %s\nCompiled on %s %s\n",
                NAME, VERSION, __DATE__, __TIME__);
        exit(0);
    case 'h':
        mqtt_host = optarg;
        str = strrchr(optarg, ':');
        if (str > mqtt_host && *(str-1) != ']') {
            /* TCP port provided */
            *str = 0;
            mqtt_port = strtoul(str+1, NULL, 10);
        }
        break;

    default:
        fprintf(stderr, "unknown option '%c'", opt);
    case '?':
        fputs(help_msg, stderr);
        exit(1);
        break;
    }

    if (atexit(cleanup) != 0) {
        fprintf(stderr, "atexit: Call failed\n");
        exit(EXIT_FAILURE);
    }

    if (!argv[optind]) {
        fprintf(stderr, "no tty given\n");
        fputs(help_msg, stderr);
        exit(1);
    }

    /* prepare program */
    ttydev = argv[optind];

    mb_connect(ttydev);

    if (mqtt_qos < 0) {
        mqtt_qos = !strcmp(mqtt_host ?: "", "localhost") ? 0 : 1;
    }
    mosquitto_lib_init();
    sprintf(mqtt_name, "%s-%i", NAME, getpid());
	mosq = mosquitto_new(mqtt_name, true, NULL);
	if (mosq == NULL) {
        fprintf(stderr, "mosquitto_new: Call failed\n");
        exit(EXIT_FAILURE);
    }

    mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
    mosquitto_message_callback_set(mosq, mqtt_message_callback);

    if (mosquitto_connect(mosq, mqtt_host, mqtt_port, mqtt_keepalive) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mosquitto_connect: Unable to connect\n");
        exit(EXIT_FAILURE);
    }

    mosquitto_loop_forever(mosq, -1, 1);

    exit(EXIT_SUCCESS);
}
