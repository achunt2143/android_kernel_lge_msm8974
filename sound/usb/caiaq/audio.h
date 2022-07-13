<<<<<<< HEAD
#ifndef CAIAQ_AUDIO_H
#define CAIAQ_AUDIO_H

int snd_usb_caiaq_audio_init(struct snd_usb_caiaqdev *dev);
void snd_usb_caiaq_audio_free(struct snd_usb_caiaqdev *dev);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef CAIAQ_AUDIO_H
#define CAIAQ_AUDIO_H

int snd_usb_caiaq_audio_init(struct snd_usb_caiaqdev *cdev);
void snd_usb_caiaq_audio_free(struct snd_usb_caiaqdev *cdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* CAIAQ_AUDIO_H */
