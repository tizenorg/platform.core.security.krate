/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __CAPI_KRATE_APP_PROXY_H__
#define __CAPI_KRATE_APP_PROXY_H__

#include <app_info.h>
#include <app_control.h>
#include <app_manager.h>

#include <krate/krate-manager.h>

/**
 * @file app-info.h
 * @brief This file provides APIs to get the information of applications in the krate
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_KRATE_APPLICATION_INFO_MODULE
 * @{
 */

/**
 * @brief       The krate application manager handle
 * @since_tizen 3.0
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_destroy()
 */
typedef void* krate_app_proxy_h;

/**
 * @brief       Acquires the krate application manager handle
 * @details     This API acquires krate application manager handle required to call
 *              the krate application manager APIs.
 * @since_tizen 3.0
 * @param[in]   manager The krate manager handle
 * @param[in]   name The krate name
 * @param[out]  handle The krate app proxy handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_CONNECTION_REFUSED Connection refused
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @see         krate_manager_create()
 * @see         krate_app_proxy_create()
 * @see         get_last_result()
 */
KRATE_API int krate_app_proxy_create(krate_manager_h manager, const char* name, krate_app_proxy_h* handle);

/**
 * @brief       Releases the krate application manager handle
 * @details     This API must be called if interaction with the krate application
 *              manager handle is no longer required.
 * @since_tizen 3.0
 * @param[in]   context The device policy manager context
 * @param[in]   handle The krate application manager handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_app_proxy_create()
 * @see         krate_app_proxy_create()
 */
KRATE_API int krate_app_proxy_destroy(krate_app_proxy_h handle);

/**
 * @brief       Gets the application information for the given application in the krate.
 * @details     This API gets application information handle required to get
 *              information of the application in the krate
 * @since_tizen 3.0
 * @param[in]   handle The krate application manager handle
 * @param[in]   appid The application ID
 * @return      Zone application handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #KRATE_ERROR_NONE No error
 * @exception   #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #KRATE_ERROR_TIMED_OUT Time out
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_create()
 * @see         krate_application_release()
 * @see         application_manager_get_application_info()
 * @see         get_last_result()
 */
KRATE_API int krate_app_proxy_get_app_info(krate_app_proxy_h handle, const char* appid, app_info_h* app_info);

/**
 * @brief       Retrieves all the handles of the application in the krate.
 * @details     This API calls app_manager_app_info_cb() once for each application
 *              ID with traversing the installed application list in the krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate application manager handle
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by krate_app_proxy_create().
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_create()
 * @see         application_manager_foreach_app_info()
 */
KRATE_API int krate_app_proxy_foreach_app_info(krate_app_proxy_h handle, app_manager_app_info_cb callback, void *user_data);

/**
 * @brief       Launch the application located at the given path into the krate.
 * @details     Administrator can use this API to launch the application in the
 *              krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate application manager handle
 * @param[in]   app_control The app_control handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_NO_SUCH_FILE No such application file
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the krate
 * @pre         The handle must be created by krate_app_proxy_create().
 * @pre         The krate corresponding to the given name must be
 *              created before use of this API.
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_destroy()
 * @see         krate_app_proxy_resume()
 * @see         krate_app_proxy_terminate()
 * @see         krate_is_running_app()
 * @see         app_control_send_launch_request()
 */
KRATE_API int krate_app_proxy_send_launch_request(krate_app_proxy_h handle, app_control_h app_control);

/**
 * @brief       Terminate the application located at the given path into the krate.
 * @details     Administrator can use this API to terminate the application in
 *              the krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate application manager handle
 * @param[in]   appid The application ID to be terminated
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_NO_SUCH_FILE No such application file
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the krate
 * @pre         The handle must be created by krate_app_proxy_create().
 * @pre         The krate corresponding to the given name must be
 *              created before use of this API.
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_destroy()
 * @see         krate_app_proxy_resume()
 * @see         krate_app_proxy_send_launch_request()
 * @see         krate_is_running_app()
 * @see         app_control_send_terminate_request()
 * @see         app_manager_send_terminate_bt_app()
 */
KRATE_API int krate_app_proxy_terminate(krate_app_proxy_h handle, const char* appid);

/**
 * @brief       Resume the application located at the given path into the krate.
 * @details     Administrator can use this API to resume the application in the
 *              krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate application manager handle
 * @param[in]   appid The application ID to be resumed
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_NO_SUCH_FILE No such application file
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the krate
 * @pre         The handle must be created by krate_app_proxy_create().
 * @pre         The krate corresponding to the given name must be
 *              created before use of this API.
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_destroy()
 * @see         krate_app_proxy_send_launch_request()
 * @see         krate_app_proxy_terminate()
 * @see         app_manager_resume_app()
 */
KRATE_API int krate_app_proxy_resume(krate_app_proxy_h handle, const char* appid);

/**
 * @brief       Checks whether the application in the krate is running.
 * @details     This API can be used to check whether the application in the krate
 *              is running.
 * @since_tizen 3.0
 * @param[in]   handle The krate application manager handle
 * @param[in]   appid The application ID
 * @param[out]  result true if the application is running,
 *              otherwise false if the application is not running
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_NO_DATA No such pacakge to check
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_app_proxy_create().
 * @see         krate_app_proxy_create()
 * @see         krate_app_proxy_destroy()
 * @see         krate_app_proxy_send_launch_request()
 * @see         krate_app_proxy_terminate()
 * @see         app_manager_is_running_app()
 */
KRATE_API int krate_app_proxy_is_running(krate_app_proxy_h handle, const char* appid, int* result);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_KRATE_APP_PROXY_H__  */
