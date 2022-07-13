<<<<<<< HEAD
#ifndef CAIAQ_INPUT_H
#define CAIAQ_INPUT_H

void snd_usb_caiaq_input_dispatch(struct snd_usb_caiaqdev *dev, char *buf, unsigned int len);
int snd_usb_caiaq_input_init(struct snd_usb_caiaqdev *dev);
void snd_usb_caiaq_input_free(struct snd_usb_caiaqdev *dev);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef CAIAQ_INPUT_H
#define CAIAQ_INPUT_H

void snd_usb_caiaq_input_dispatch(struct snd_usb_caiaqdev *cdev, char *buf, unsigned int len);
int snd_usb_caiaq_input_init(struct snd_usb_caiaqdev *cdev);
void snd_usb_caiaq_input_free(struct snd_usb_caiaqdev *cdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
