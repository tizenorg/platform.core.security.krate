/*
 * Tizen Krate launcher application
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef CONF_H_
#define CONF_H_

#define RESOLUTION_SCALE_W      1.0
#define RESOLUTION_SCALE_H      1.0

#define ICON_IMG_SIZE           ((118) * (RESOLUTION_SCALE_W))
#define ICON_TXT_SIZE_NORMAL    ((int)((28) * (RESOLUTION_SCALE_H)))
#define ICON_TXT_COLOR          "000000FF"
#define ICON_TXT_SHADOW_COLOR   "88888888"

#define ICON_SIZE_W		((720) * (RESOLUTION_SCALE_W) / 4)
#define ICON_SIZE_H		((1060) * (RESOLUTION_SCALE_H) / 5)

#define MAX_BADGE_DISPLAY_COUNT 999

#define ALLOWED_ICON_DRAG_DISTANCE	100
#define LONG_PRESS_TIME		0.75

#endif /* CONF_H_ */
