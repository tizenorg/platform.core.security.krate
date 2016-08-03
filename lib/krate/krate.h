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

#ifndef __KRATE_KRATE_MANAGER_H__
#define __KRATE_KRATE_MANAGER_H__

#include <tizen.h>

/**
 * @file krate.h
 * @brief This file defines common data types required to krate APIs.
 */

#ifndef KRATE_API
#define KRATE_API __attribute__((visibility("default")))
#endif // API

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_KRATE_KRATE_MANAGER_MODULE
 * @{
 */

/**
 * @brief       Enumeration of device policy API errors
 * @since_tizen 3.0
 */
typedef enum {
	KRATE_ERROR_NONE                 = TIZEN_ERROR_NONE,                 /**< The operation was successful */
	KRATE_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
	KRATE_ERROR_CONNECTION_REFUSED   = TIZEN_ERROR_CONNECTION_REFUSED,   /**< Connection refused */
	KRATE_ERROR_TIMED_OUT            = TIZEN_ERROR_TIMED_OUT,            /**< Time out */
	KRATE_ERROR_PERMISSION_DENIED    = TIZEN_ERROR_PERMISSION_DENIED,    /**< Access privilege is not sufficient */
	KRATE_ERROR_NOT_SUPPORTED        = TIZEN_ERROR_NOT_SUPPORTED,        /**< Operation is not supported */
	KRATE_ERROR_NO_SUCH_FILE         = TIZEN_ERROR_NO_SUCH_FILE,         /**< No such file or directory */
	KRATE_ERROR_FILE_EXISTS          = TIZEN_ERROR_FILE_EXISTS,          /**< File exists */
	KRATE_ERROR_OUT_OF_MEMORY        = TIZEN_ERROR_OUT_OF_MEMORY,        /**< Out of memory */
	KRATE_ERROR_NO_DATA              = TIZEN_ERROR_NO_DATA               /**< No Data */
} krate_error_type_e;

/**
 * @brief       The krate manager handle
 * @details     The krate manager  handle is an abstraction of the
 *              logical connection between the krate manager and it's client.
 *              The krate manager handle must be created by using
 *              krate_manager_create() before attempting to use almost any of
 *              the krate related APIs, and it should be freed when interaction
 *              with the krate manager is no longer required.
 *              To release the handle, use krate_manager_destroy().
 * @since_tizen 3.0
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 */
typedef void* krate_manager_h;

/**
 * @brief       Creates the krate manager handle.
 * @details     This API creates krate manager handle required to
 *              the krate related APIs.
 *              This API is also used to verify whether caller is authorized
 *              or not.
 * @since_tizen 3.0
 * @param[out]  handle The krate manager handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_CONNECTION_REFUSED Connection refused
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @see         krate_manager_destroy()
 * @see         get_last_result()
 */
KRATE_API int krate_manager_create(krate_manager_h *handle);

/**
 * @brief       Releases the krate manager handle.
 * @details     This API must be called if interaction with the krate manager is
 *              no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The krate manager handle
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by krate_manager_create()
 * @see         krate_manager_create()
 */
KRATE_API int krate_manager_destroy(krate_manager_h handle);

/**
 * @brief       Called when a krate raises a event.
 * @since_tizen 3.0
 * @param[in]   name The krate name
 * @param[in]   object The object name triggered the event
 * @param[in]   user_data The user data passed from krate_manager_add_event_cb
 * @see         krate_manager_add_event_cb()
 * @see         krate_manager_remove_event_cb()
 */
typedef void(*krate_event_cb)(const char* name, const char* object, void* user_data);

/**
 * @brief       Adds krate event callback.
 * @details     This API can be used to receive events sent by krate
 *              The callback specified to this function is automatically called when
 *              the krate is created or removed.
 * @since_tizen 3.0
 * @param[in]   context The krate_manager handle
 * @param[in]   event The event name to receive
 * @param[in]   callback The event callback
 * @param[in]   user_data The user data passed to the callback function
 * @param[out]  id Signal identifier
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_manager_create().
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 * @see         krate_manager_remove_event_cb()
 */
KRATE_API int krate_manager_add_event_cb(krate_manager_h handle,
									  const char* event,
									  krate_event_cb callback, void* user_data,
									  int* id);

/**
 * @brief       Removes krate event callback.
 * @details     This API removes krate event callback
 * @since_tizen 3.0
 * @param[in]   context The krate manager handle
 * @param[in]   id Signal identifier
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The context must be created by krate_manager_create().
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 * @see         krate_manager_add_event_cb()
 */
KRATE_API int krate_manager_remove_event_cb(krate_manager_h handle, int id);

/**
 * @brief       Creates a new krate.
 * @details     This API creates a container. All file system objects neeeded
 *              will be also created. manifest XML passed by parameter will be
 *              used when the krate is running.
 * @since_tizen 3.0
 * @param[in]   handle The krate manager handle
 * @param[in]   name The krate name to be created
 * @param[in]   manifest The manifest XML to be used when the krate is runned.
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by krate_manager_create().
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 * @see         krate_manager_destroy_krate()
 * @see         krate_manager_create_krate_iterator()
 */
KRATE_API int krate_manager_create_krate(krate_manager_h handle,
											const char* name,
											const char* manifest);

/**
 * @brief       Removes existing krate.
 * @details     This removes krate. All file system objects created for the krate
 *              will be also erased.
 * @since_tizen 3.0
 * @param[in]   handle The krate manager handle
 * @param[in]   name The krate name to be removed
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the krate
 * @pre         The handle must be created by krate_manager_create().
 * @pre         The krate corresponding to the given name must be
 *              created before use of this API.
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 * @see         krate_manager_create_krate()
 * @see         krate_manager_create_krate_iterator()
 */
KRATE_API int krate_manager_destroy_krate(krate_manager_h handle,
											const char* name);

/*
 * @brief       Enumeration for krate state
 * @since_tizen 3.0
 */
typedef enum {
	KRATE_STATE_LOCKED       = 0x01,  /**< Krate has been defined, but it can not start. */
	KRATE_STATE_RUNNING      = 0x02, /**< Krate has been started. */
	KRATE_STATE_ALL          = 0xff  /**< This presents all of the state  */
} krate_state_e;

/**
 * @brief       Gets the krate state.
 * @details     This API can be used to get the state of the krate. The krate can
 *              have one of the three states(running, locked).
 * @since_tizen 3.0
 * @param[in]   handle The krate policy handle
 * @param[in]   name The krate name
 * @param[out]  state The krate state
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_NO_DATA No such krate to get state
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_manager_create().
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 * @see         krate_manager_create_krate()
 * @see         krate_manager_destroy_krate()
 */
KRATE_API int krate_manager_get_krate_state(krate_manager_h handle,
											const char* name,
											krate_state_e* state);

/**
 * @brief       Called to get all the name of created krates.
 * @since_tizen 3.0
 * @param[in]   name The krate name
 * @param[in]   user_data The user data passed from krate_manager_foreach_name
 * @return      true to continue with the next iteration of the loop, otherwise false to break out out the loop
 * @see         krate_manager_foreach_name()
 */
typedef bool(*krate_manager_foreach_cb)(const char* name, void* user_data);

/**
 * @brief       Retrieves all the name of created krates
 * @details     This API calls krate_manager_foreach_cb() once for each krate name
 *              with traversing the created krates list.
 * @since_tizen 3.0
 * @param[in]   handle The krate policy handle
 * @param[in]   state a combination of the krate state to look
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by krate_manager_create().
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 * @see         krate_manager_create_krate()
 * @see         krate_manager_destroy_krate()
 */
KRATE_API int krate_manager_foreach_name(krate_manager_h handle,
										   krate_state_e state,
										   krate_manager_foreach_cb callback,
										   void* user_data);

/**
 * @brief       Reset password for the krate.
 * @details     This API can be used to set password for the krate. The password
 *              of the krate can be used for authenticating user.
 * @since_tizen 3.0
 * @param[in]   handle The krate policy handle
 * @param[in]   name The krate name
 * @param[out]  new_password new password if NULL, existing password will be removed
 * @return      #KRATE_ERROR_NONE on success, otherwise a negative value
 * @retval      #KRATE_ERROR_NONE Successful
 * @retval      #KRATE_ERROR_NO_DATA No such krate to get state
 * @retval      #KRATE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #KRATE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by krate_manager_create().
 * @see         krate_manager_create()
 * @see         krate_manager_destroy()
 */
KRATE_API int krate_manager_reset_krate_password(krate_manager_h handle,
									const char* name, const char* new_password);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __KRATE_KRATE_MANAGER_H__ */
