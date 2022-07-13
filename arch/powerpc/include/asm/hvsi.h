<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _HVSI_H
#define _HVSI_H

#define VS_DATA_PACKET_HEADER           0xff
#define VS_CONTROL_PACKET_HEADER        0xfe
#define VS_QUERY_PACKET_HEADER          0xfd
#define VS_QUERY_RESPONSE_PACKET_HEADER 0xfc

/* control verbs */
#define VSV_SET_MODEM_CTL    1 /* to service processor only */
#define VSV_MODEM_CTL_UPDATE 2 /* from service processor only */
#define VSV_CLOSE_PROTOCOL   3

/* query verbs */
#define VSV_SEND_VERSION_NUMBER 1
#define VSV_SEND_MODEM_CTL_STATUS 2

/* yes, these masks are not consecutive. */
#define HVSI_TSDTR 0x01
#define HVSI_TSCD  0x20

#define HVSI_MAX_OUTGOING_DATA 12
#define HVSI_VERSION 1

struct hvsi_header {
	uint8_t  type;
	uint8_t  len;
<<<<<<< HEAD
	uint16_t seqno;
=======
	__be16 seqno;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} __attribute__((packed));

struct hvsi_data {
	struct hvsi_header hdr;
	uint8_t  data[HVSI_MAX_OUTGOING_DATA];
} __attribute__((packed));

struct hvsi_control {
	struct hvsi_header hdr;
<<<<<<< HEAD
	uint16_t verb;
	/* optional depending on verb: */
	uint32_t word;
	uint32_t mask;
=======
	__be16 verb;
	/* optional depending on verb: */
	__be32 word;
	__be32 mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} __attribute__((packed));

struct hvsi_query {
	struct hvsi_header hdr;
<<<<<<< HEAD
	uint16_t verb;
=======
	__be16 verb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} __attribute__((packed));

struct hvsi_query_response {
	struct hvsi_header hdr;
<<<<<<< HEAD
	uint16_t verb;
	uint16_t query_seqno;
	union {
		uint8_t  version;
		uint32_t mctrl_word;
=======
	__be16 verb;
	__be16 query_seqno;
	union {
		uint8_t  version;
		__be32 mctrl_word;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} u;
} __attribute__((packed));

/* hvsi lib struct definitions */
#define HVSI_INBUF_SIZE		255
struct tty_struct;
struct hvsi_priv {
	unsigned int	inbuf_len;	/* data in input buffer */
	unsigned char	inbuf[HVSI_INBUF_SIZE];
	unsigned int	inbuf_cur;	/* Cursor in input buffer */
<<<<<<< HEAD
	unsigned int	inbuf_pktlen;	/* packet lenght from cursor */
=======
	size_t		inbuf_pktlen;	/* packet length from cursor */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_t	seqno;		/* packet sequence number */
	unsigned int	opened:1;	/* driver opened */
	unsigned int	established:1;	/* protocol established */
	unsigned int 	is_console:1;	/* used as a kernel console device */
	unsigned int	mctrl_update:1;	/* modem control updated */
	unsigned short	mctrl;		/* modem control */
	struct tty_struct *tty;		/* tty structure */
<<<<<<< HEAD
	int (*get_chars)(uint32_t termno, char *buf, int count);
	int (*put_chars)(uint32_t termno, const char *buf, int count);
=======
	ssize_t (*get_chars)(uint32_t termno, u8 *buf, size_t count);
	ssize_t (*put_chars)(uint32_t termno, const u8 *buf, size_t count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	uint32_t	termno;
};

/* hvsi lib functions */
struct hvc_struct;
extern void hvsilib_init(struct hvsi_priv *pv,
<<<<<<< HEAD
			 int (*get_chars)(uint32_t termno, char *buf, int count),
			 int (*put_chars)(uint32_t termno, const char *buf,
					  int count),
=======
			 ssize_t (*get_chars)(uint32_t termno, u8 *buf,
					      size_t count),
			 ssize_t (*put_chars)(uint32_t termno, const u8 *buf,
					      size_t count),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 int termno, int is_console);
extern int hvsilib_open(struct hvsi_priv *pv, struct hvc_struct *hp);
extern void hvsilib_close(struct hvsi_priv *pv, struct hvc_struct *hp);
extern int hvsilib_read_mctrl(struct hvsi_priv *pv);
extern int hvsilib_write_mctrl(struct hvsi_priv *pv, int dtr);
extern void hvsilib_establish(struct hvsi_priv *pv);
<<<<<<< HEAD
extern int hvsilib_get_chars(struct hvsi_priv *pv, char *buf, int count);
extern int hvsilib_put_chars(struct hvsi_priv *pv, const char *buf, int count);
=======
extern ssize_t hvsilib_get_chars(struct hvsi_priv *pv, u8 *buf, size_t count);
extern ssize_t hvsilib_put_chars(struct hvsi_priv *pv, const u8 *buf,
				 size_t count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _HVSI_H */
