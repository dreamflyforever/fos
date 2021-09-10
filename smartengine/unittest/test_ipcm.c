/*
	mosquitto_pub -h 0.0.0.0 -p 1883 -t world -m world
	mosquitto_pub -h 0.0.0.0 -p 1883 -t hello -m hello
*/

#include "mqtt_mid.h"

int mq_fd;
int count;
int pass;

int mqtt_hello_cb(const uint8_t *data, int len);
int mqtt_world_cb(const uint8_t *data, int len);

struct TEST_CASE {
	char *input_topic;
	MQCB input_cb;
	char *out_data;
	int out_len;
	char *expect_data;
	int expect_len;
};

struct TEST_CASE tests[] = {
	{
		.input_topic = "hello",
		.input_cb = mqtt_hello_cb,
		.expect_data = "hello",
		.expect_len = 5,
	},
	{
		.input_topic = "world",
		.input_cb = mqtt_world_cb,
		.expect_data = "world",
		.expect_len = 5,
	}
};

/* example */
int mqtt_hello_cb(const uint8_t *data, int len)
{
	LOG(LL_INFO, ("CB:666:%.*s", len, data));
	tests[0].out_data = malloc(len);
	strncpy(tests[0].out_data, data, len);
	tests[0].out_len = len;
	return 0;
}

int mqtt_world_cb(const uint8_t *data, int len)
{
	LOG(LL_INFO, ("CB:usb:%.*s", len, data));
	tests[1].out_data = malloc(len);
	strncpy(tests[1].out_data, data, len);
	tests[1].out_len = len;
	return 0;
}

int sub_add(struct TEST_CASE test)
{
	int retval = 0;
	mqtt_sub_cb(mq_fd, test.input_topic, test.input_cb);

	return retval;
}

int print_arg(struct TEST_CASE tc)
{
	printf("expect data: %s\toutout data: %s\t ||", tc.out_data, tc.expect_data);
	printf("\texpect len: %d\toutput len: %d\t", tc.out_len, tc.expect_len);
	printf("\n");
	return 0;
}

int free_arg(struct TEST_CASE tc)
{
	free(tc.out_data);
	return 0;
}

int check_expect(struct TEST_CASE tc)
{
	int retval = 0;
	print_arg(tc);
	if (tc.out_data == NULL) {
		goto error;
	}
	if ( (0 == strncmp(tc.out_data,
				tc.expect_data,
				tc.expect_len))
				&& (tc.out_len == tc.expect_len)) {
		retval = 1;
	}
error:
	return retval;
}

#define CHECK_EXPECT(a) \
	count++;\
	pass += check_expect(a);

int num = 0;
int main(void)
{
	mq_fd = mqtt_create();

	int i;
	for (i = 0; i < 2; i++) {
		sub_add(tests[i]);
	}
	sleep(10);

	printf("\n===============================================================================\n\n");

	for (i = 0; i < 2; i++) {
		CHECK_EXPECT(tests[i]);
	}

	printf("All Test Done!\n\n");
	printf("Result: %2d/%2d [PASS/TOTAL]\n\n", pass, count);
	printf("\n===============================================================================\n\n");
	
	for (i = 0; i < 2; i++) {
		free_arg(tests[i]);
	}
	while(1) {
		
		sleep(1);
		num++;
		if (num >= 2) {
			mqtt_del(mq_fd);
			return 0;
		}
	}
}
