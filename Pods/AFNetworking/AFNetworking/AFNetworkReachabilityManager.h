// AFNetworkReachabilityManager.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>

#if !TARGET_OS_WATCH
#import <SystemConfiguration/SystemConfiguration.h>

//网络状态
typedef NS_ENUM(NSInteger, AFNetworkReachabilityStatus) {
    AFNetworkReachabilityStatusUnknown          = -1,//未知
    AFNetworkReachabilityStatusNotReachable     = 0,//无网络
    AFNetworkReachabilityStatusReachableViaWWAN = 1,//运营商网络
    AFNetworkReachabilityStatusReachableViaWiFi = 2,//WiFi网络
};

//批量非空标识
NS_ASSUME_NONNULL_BEGIN

/**
 `AFNetworkReachabilityManager` monitors the reachability of domains, and addresses for both WWAN and WiFi network interfaces.

 Reachability can be used to determine background information about why a network operation failed, or to trigger a network operation retrying when a connection is established. It should not be used to prevent a user from initiating a network request, as it's possible that an initial request may be required to establish reachability.

 See Apple's Reachability Sample Code ( https://developer.apple.com/library/ios/samplecode/reachability/ )

 @warning Instances of `AFNetworkReachabilityManager` must be started with `-startMonitoring` before reachability status can be determined.
 */
@interface AFNetworkReachabilityManager : NSObject

/**
    当前网络状态
 */
@property (readonly, nonatomic, assign) AFNetworkReachabilityStatus networkReachabilityStatus;

/**
    当前是否有网络
 */
@property (readonly, nonatomic, assign, getter = isReachable) BOOL reachable;

/**
    当前是否为运营商网络
 */
@property (readonly, nonatomic, assign, getter = isReachableViaWWAN) BOOL reachableViaWWAN;

/**
    当前是否为WiFi
 */
@property (readonly, nonatomic, assign, getter = isReachableViaWiFi) BOOL reachableViaWiFi;

///---------------------
/// @name 初始化
///---------------------

/**
    全局单例
 */
+ (instancetype)sharedManager;

/**
    工厂方法
 */
+ (instancetype)manager;

/**
    监控domain域的管理器
 */
+ (instancetype)managerForDomain:(NSString *)domain;

/**
    监听某个socket地址的管理器
 */
+ (instancetype)managerForAddress:(const void *)address;

/**
    通过SCNetworkReachabilityRef对象初始化、其余所有的方法也会汇聚于此
 */
- (instancetype)initWithReachability:(SCNetworkReachabilityRef)reachability NS_DESIGNATED_INITIALIZER;

///--------------------------------------------------
/// @name 开始和关闭
///--------------------------------------------------

/**
    打开监听
 */
- (void)startMonitoring;

/**
    关闭监听
 */
- (void)stopMonitoring;

///-------------------------------------------------
/// @name Getting Localized Reachability Description
///-------------------------------------------------

/**
    返回网络状态的字符串
 */
- (NSString *)localizedNetworkReachabilityStatusString;

///---------------------------------------------------
/// @name Setting Network Reachability Change Callback
///---------------------------------------------------

/**
    设置网络状态改变时的回调
    你也可以通过监听`AFNetworkingReachabilityDidChangeNotification`中的[AFNetworkingReachabilityNotificationStatusItem]来获取
 */
- (void)setReachabilityStatusChangeBlock:(nullable void (^)(AFNetworkReachabilityStatus status))block;

@end

///----------------
/// @name Constants
///----------------

/**
 ## Network Reachability

 The following constants are provided by `AFNetworkReachabilityManager` as possible network reachability statuses.

 enum {
 AFNetworkReachabilityStatusUnknown,
 AFNetworkReachabilityStatusNotReachable,
 AFNetworkReachabilityStatusReachableViaWWAN,
 AFNetworkReachabilityStatusReachableViaWiFi,
 }

 `AFNetworkReachabilityStatusUnknown`
 The `baseURL` host reachability is not known.

 `AFNetworkReachabilityStatusNotReachable`
 The `baseURL` host cannot be reached.

 `AFNetworkReachabilityStatusReachableViaWWAN`
 The `baseURL` host can be reached via a cellular connection, such as EDGE or GPRS.

 `AFNetworkReachabilityStatusReachableViaWiFi`
 The `baseURL` host can be reached via a Wi-Fi connection.

 ### Keys for Notification UserInfo Dictionary

 Strings that are used as keys in a `userInfo` dictionary in a network reachability status change notification.

 `AFNetworkingReachabilityNotificationStatusItem`
 A key in the userInfo dictionary in a `AFNetworkingReachabilityDidChangeNotification` notification.
 The corresponding value is an `NSNumber` object representing the `AFNetworkReachabilityStatus` value for the current reachability status.
 */

///--------------------
/// @name Notifications
///--------------------

/**
 Posted when network reachability changes.
 This notification assigns no notification object. The `userInfo` dictionary contains an `NSNumber` object under the `AFNetworkingReachabilityNotificationStatusItem` key, representing the `AFNetworkReachabilityStatus` value for the current network reachability.

 @warning In order for network reachability to be monitored, include the `SystemConfiguration` framework in the active target's "Link Binary With Library" build phase, and add `#import <SystemConfiguration/SystemConfiguration.h>` to the header prefix of the project (`Prefix.pch`).
 */

// FOUNDATION_EXPORT 和#define 都能定义常量。FOUNDATION_EXPORT 并且能够隐藏实现细节
FOUNDATION_EXPORT NSString * const AFNetworkingReachabilityDidChangeNotification;
FOUNDATION_EXPORT NSString * const AFNetworkingReachabilityNotificationStatusItem;

///--------------------
/// @name Functions
///--------------------

/**
 Returns a localized string representation of an `AFNetworkReachabilityStatus` value.
 */
FOUNDATION_EXPORT NSString * AFStringFromNetworkReachabilityStatus(AFNetworkReachabilityStatus status);

NS_ASSUME_NONNULL_END
#endif
