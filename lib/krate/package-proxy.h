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

#ifndef __CAPI_ZONE_PACKAGE_INFO_H__
#define __CAPI_ZONE_PACKAGE_INFO_H__

#include <package_info.h>
#include <package_manager.h>

#include <zone/zone.h>

/**
 * @file package-info.h
 * @brief This file provides APIs to get the information of packages in the zone
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_ZONE_ZONE_PACKAGE_INFO_MODULE
 * @{
 */

/**
 * @brief       The zone package proxy handle
 * @since_tizen 3.0
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 */
typedef void* zone_package_proxy_h;

/**
 * @brief       Acquires the zone package proxy handle.
 * @details     This API acquires zone package proxy handle required to call
 *              the zone package_manager APIs.
 * @since_tizen 3.0
 * @param[in]   manager The zone manager handle
 * @param[in]   name The zone name
 * @param[out]  handle The zone package proxy handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_CONNECTION_REFUSED Connection refused
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @see         zone_manager_create()
 * @see         zone_package_proxy_destroy()
 * @see         get_last_result()
 */
ZONE_API int zone_package_proxy_create(zone_manager_h manager, const char* name, zone_package_proxy_h *handle);

/**
 * @brief       Releases the zone package proxy handle.
 * @details     This API must be called if interaction with the zone package
 *              manager handle is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The zone package proxy handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by zone_package_proxy_create()
 * @see         zone_package_proxy_create()
 */
ZONE_API int zone_package_proxy_destroy(zone_package_proxy_h handle);

/**
 * @brief       Gets the package information for the given package in the zone.
 * @details     This API gets package information handle required to get
 *              information of the pacakge in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package proxy handle
 * @param[in]   pakcage_id The package ID
 * @return      Zone package information handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @remark      The handle should be destroyed using package_info_destroy().
 * @exception   #ZONE_ERROR_NONE No error
 * @exception   #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #ZONE_ERROR_TIMED_OUT Time out
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         package_manager_get_package_info()
 * @see         package_manager_destroy()
 * @see         get_last_result()
 */
ZONE_API int zone_package_proxy_get_package_info(zone_package_proxy_h handle, const char* pakcage_id, package_info_h* package_info);

/**
 * @brief       Retrieves all the IDs of the installed package in the zone.
 * @details     This API calls package_manager_package_info_cb() once for
 *              each package information handle with traversing the installed
 *              package list in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package proxy handle
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by zone_package_proxy_create().
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         package_manager_foreach_package_info()
 */
ZONE_API int zone_package_proxy_foreach_package_info(zone_package_proxy_h handle, package_manager_package_info_cb callback, void *user_data);

/**
 * @brief       Installs the package located at the given path into the zone.
 * @details     Administrator can use this API to install the package into the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package proxy handle
 * @param[in]   name The zone name
 * @param[in]   package_path The absolute path to the package to be installed
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_NO_SUCH_FILE No such package file
 * @retval      #ZONE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by zone_package_proxy_create().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         zone_package_proxy_uninstall()
 * @see         package_manager_request_install()
 */
ZONE_API int zone_package_proxy_install(zone_package_proxy_h handle, const char* package_path);

/**
 * @brief       Uinstalls the package with the given ID from the zone.
 * @details     Administrator can use this API to uninstall the package from the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package proxy handle
 * @param[in]   name The zone name
 * @param[in]   pakcage_id The pakcage ID
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_NO_DATA No such pacakge to unintall
 * @retval      #ZONE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by zone_package_proxy_create().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         zone_package_proxy_install()
 * @see         package_manager_request_uninstall()
 */
ZONE_API int zone_package_proxy_uninstall(zone_package_proxy_h handle, const char* pakcage_id);

/**
 * @brief       Sets the event status that presents the package has been
 *              installed, uninstalled or updated in the zone.
 * @details     This API sets the event status of the package for monitoring
 *              whether the package has been installed, uninstalled or updated.
 *               event status can be combined multiple status using OR operation.
 * @since_tizen 3.0
 * @param[in]   handle The zone package proxy handle
 * @param[in]   status_type The status of the package
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by zone_package_proxy_create().
 * @pre         the zone corresponding to the given name must be
 *              created before use of this api.
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         zone_package_proxy_uninstall()
 * @see         zone_package_proxy_set_event_cb()
 * @see         zone_package_proxy_unset_event_cb()
 * @see         package_manager_set_event_status()
 */
ZONE_API int zone_package_proxy_set_event_status(zone_package_proxy_h handle, int status_type);

/**
 * @brief       Registers a callback function for package event.
 * @details     This API sets a callback function to be invoked when the package
 *              has been installed, uninstalled or updated.
 * @since_tizen 3.0
 * @param[in]   handle the zone package proxy handle
 * @param[in]   callback The callback function to be registered
 * @param[in]   user_data The user data to be passed to the callback function
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         the handle must be created by zone_package_proxy_create().
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         zone_package_proxy_uninstall()
 * @see         zone_package_proxy_set_event_status()
 * @see         zone_package_proxy_unset_event_cb()
 * @see         package_manager_event_cb()
 * @see         package_manager_set_event_cb()
 */
ZONE_API int zone_package_proxy_set_event_cb(zone_package_proxy_h handle, package_manager_event_cb callback, void *user_data);

/**
 * @brief       Unregisters the callback function.
 * @details     This API unsets the callback funtion not to be invoked by
 *              package event status
 * @since_tizen 3.0
 * @param[in]   handle the zone package proxy handle
 * @param[in]   name the zone name
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         the handle must be created by zone_package_proxy_create().
 *              created before use of this api.
 * @see         zone_package_proxy_create()
 * @see         zone_package_proxy_destroy()
 * @see         zone_package_proxy_uninstall()
 * @see         zone_package_proxy_set_event_status()
 * @see         zone_package_proxy_set_event_cb()
 * @see         package_manager_unset_event_cb()
 */
ZONE_API int zone_package_proxy_unset_event_cb(zone_package_proxy_h handle);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_ZONE_PACKAGE_INFO_H__ */
