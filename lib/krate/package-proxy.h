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

#ifndef __CAPI_KRATE_PACKAGE_INFO_H__
#define __CAPI_KRATE_PACKAGE_INFO_H__

#include <package_info.h>
#include <package_manager.h>

#include <krate/krate.h>

/**
 * @file package-info.h
 * @brief This file provides APIs to get the information of packages in the krate
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_KRATE_KRATE_PACKAGE_INFO_MODULE
 * @{
 */

/**
 * @brief       The krate package proxy handle
 * @since_tizen 3.0
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 */
typedef void* krate_package_proxy_h;

/**
 * @brief       Acquires the krate package proxy handle.
 * @details     This API acquires krate package proxy handle required to call
 *              the krate package_manager APIs.
 * @since_tizen 3.0
 * @param[in]   manager The krate manager handle
 * @param[in]   name The krate name
 * @param[out]  handle The krate package proxy handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_CONNECTION_REFUSED Connection refused
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @see         krate_manager_create()
 * @see         krate_package_proxy_destroy()
 * @see         get_last_result()
 */
KRATE_API int krate_package_proxy_create(krate_manager_h manager, const char* name, krate_package_proxy_h *handle);

/**
 * @brief       Releases the krate package proxy handle.
 * @details     This API must be called if interaction with the krate package
 *              manager handle is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The krate package proxy handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_package_proxy_create()
 * @see         krate_package_proxy_create()
 */
KRATE_API int krate_package_proxy_destroy(krate_package_proxy_h handle);

/**
 * @brief       Gets the package information for the given package in the krate.
 * @details     This API gets package information handle required to get
 *              information of the pacakge in the krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate package proxy handle
 * @param[in]   pakcage_id The package ID
 * @return      Krate package information handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @remark      The handle should be destroyed using package_info_destroy().
 * @exception   #KRATE_ERROR_NONE No error
 * @exception   #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #KRATE_ERROR_TIMED_OUT Time out
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         package_manager_get_package_info()
 * @see         package_manager_destroy()
 * @see         get_last_result()
 */
KRATE_API int krate_package_proxy_get_package_info(krate_package_proxy_h handle, const char* pakcage_id, package_info_h* package_info);

/**
 * @brief       Retrieves all the IDs of the installed package in the krate.
 * @details     This API calls package_manager_package_info_cb() once for
 *              each package information handle with traversing the installed
 *              package list in the krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate package proxy handle
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by krate_package_proxy_create().
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         package_manager_foreach_package_info()
 */
KRATE_API int krate_package_proxy_foreach_package_info(krate_package_proxy_h handle, package_manager_package_info_cb callback, void *user_data);

/**
 * @brief       Installs the package located at the given path into the krate.
 * @details     Administrator can use this API to install the package into the
 *              krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate package proxy handle
 * @param[in]   name The krate name
 * @param[in]   package_path The absolute path to the package to be installed
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_NO_SUCH_FILE No such package file
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the krate
 * @pre         The handle must be created by krate_package_proxy_create().
 * @pre         The krate corresponding to the given name must be
 *              created before use of this API.
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         krate_package_proxy_uninstall()
 * @see         package_manager_request_install()
 */
KRATE_API int krate_package_proxy_install(krate_package_proxy_h handle, const char* package_path);

/**
 * @brief       Uinstalls the package with the given ID from the krate.
 * @details     Administrator can use this API to uninstall the package from the
 *              krate.
 * @since_tizen 3.0
 * @param[in]   handle The krate package proxy handle
 * @param[in]   name The krate name
 * @param[in]   pakcage_id The pakcage ID
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_NO_DATA No such pacakge to unintall
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the krate
 * @pre         The handle must be created by krate_package_proxy_create().
 * @pre         The krate corresponding to the given name must be
 *              created before use of this API.
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         krate_package_proxy_install()
 * @see         package_manager_request_uninstall()
 */
KRATE_API int krate_package_proxy_uninstall(krate_package_proxy_h handle, const char* pakcage_id);

/**
 * @brief       Sets the event status that presents the package has been
 *              installed, uninstalled or updated in the krate.
 * @details     This API sets the event status of the package for monitoring
 *              whether the package has been installed, uninstalled or updated.
 *               event status can be combined multiple status using OR operation.
 * @since_tizen 3.0
 * @param[in]   handle The krate package proxy handle
 * @param[in]   status_type The status of the package
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_package_proxy_create().
 * @pre         the krate corresponding to the given name must be
 *              created before use of this api.
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         krate_package_proxy_uninstall()
 * @see         krate_package_proxy_set_event_cb()
 * @see         krate_package_proxy_unset_event_cb()
 * @see         package_manager_set_event_status()
 */
KRATE_API int krate_package_proxy_set_event_status(krate_package_proxy_h handle, int status_type);

/**
 * @brief       Registers a callback function for package event.
 * @details     This API sets a callback function to be invoked when the package
 *              has been installed, uninstalled or updated.
 * @since_tizen 3.0
 * @param[in]   handle the krate package proxy handle
 * @param[in]   callback The callback function to be registered
 * @param[in]   user_data The user data to be passed to the callback function
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         the handle must be created by krate_package_proxy_create().
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         krate_package_proxy_uninstall()
 * @see         krate_package_proxy_set_event_status()
 * @see         krate_package_proxy_unset_event_cb()
 * @see         package_manager_event_cb()
 * @see         package_manager_set_event_cb()
 */
KRATE_API int krate_package_proxy_set_event_cb(krate_package_proxy_h handle, package_manager_event_cb callback, void *user_data);

/**
 * @brief       Unregisters the callback function.
 * @details     This API unsets the callback funtion not to be invoked by
 *              package event status
 * @since_tizen 3.0
 * @param[in]   handle the krate package proxy handle
 * @param[in]   name the krate name
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         the handle must be created by krate_package_proxy_create().
 *              created before use of this api.
 * @see         krate_package_proxy_create()
 * @see         krate_package_proxy_destroy()
 * @see         krate_package_proxy_uninstall()
 * @see         krate_package_proxy_set_event_status()
 * @see         krate_package_proxy_set_event_cb()
 * @see         package_manager_unset_event_cb()
 */
KRATE_API int krate_package_proxy_unset_event_cb(krate_package_proxy_h handle);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_KRATE_PACKAGE_INFO_H__ */
