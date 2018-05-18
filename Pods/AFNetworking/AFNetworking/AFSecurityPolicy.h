// AFSecurityPolicy.h
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
#import <Security/Security.h>

typedef NS_ENUM(NSUInteger, AFSSLPinningMode) {
    AFSSLPinningModeNone,//无条件信任服务器的证书
    AFSSLPinningModePublicKey,//对服务器返回的证书中的PublicKey进行验证
    AFSSLPinningModeCertificate,//对服务器返回的证书同本地证书全部进行校验
};

/**
 `AFSecurityPolicy` evaluates server trust against pinned X.509 certificates and public keys over secure connections.

 Adding pinned SSL certificates to your app helps prevent man-in-the-middle attacks and other vulnerabilities. Applications dealing with sensitive customer data or financial information are strongly encouraged to route all communication over an HTTPS connection with SSL pinning configured and enabled.
 */

NS_ASSUME_NONNULL_BEGIN

@interface AFSecurityPolicy : NSObject <NSSecureCoding, NSCopying>

/**
    SSLPinning 默认 `AFSSLPinningModeNone`
 */
@property (readonly, nonatomic, assign) AFSSLPinningMode SSLPinningMode;

/**
    本地证书合集

    默认、将会从整个工程目录下加载所有(.cer)的证书文件
    如果想定制证书、可以使用`certificatesInBundle`来加载证书
    然后调用`policyWithPinningMode:withPinnedCertificates`来创建一个新`AFSecurityPolicy`对象用于验证
 
    如果证书合集中任何一个被校验通过、那么`evaluateServerTrust:forDomain:`都将返回true
 */
@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

/**
    使用允许无效或过期的证书 默认`NO`
 */
@property (nonatomic, assign) BOOL allowInvalidCertificates;

/**
    是否验证域名 默认`YES`
 */
@property (nonatomic, assign) BOOL validatesDomainName;

///-----------------------------------------
/// @name 获取证书
///-----------------------------------------

/**
    从指定`bundle`中获取证书合集
    然后调用`policyWithPinningMode:withPinnedCertificates`来创建一个新`AFSecurityPolicy`对象用于验证
 */
+ (NSSet <NSData *> *)certificatesInBundle:(NSBundle *)bundle;

///-----------------------------------------
/// @name 自定义安全策略
///-----------------------------------------

/**
    默认的安全策略
    1、不允许无效或过期的证书
    2、验证域名
    3、不对证书和公钥进行验证
 */
+ (instancetype)defaultPolicy;

///---------------------
/// @name Initialization
///---------------------

/**
    通过指定的验证策略`AFSSLPinningMode`来创建
 */
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode;

/**
    通过指定的验证策略`AFSSLPinningMode`、以及证书合集来创建
 */
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode withPinnedCertificates:(NSSet <NSData *> *)pinnedCertificates;

///------------------------------
/// @name Evaluating Server Trust
///------------------------------

/**
    根据具体配置、确定是否接受指定服务器的信任

    服务器验证时会返回`NSURLCredential`challenge对象
    @param serverTrust 使用challenge.protectionSpace.serverTrust参数即可
    @param domain 使用challenge.protectionSpace.host即可

 */
- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END

///----------------
/// @name Constants
///----------------

/**
 ## SSL Pinning Modes

 The following constants are provided by `AFSSLPinningMode` as possible SSL pinning modes.

 enum {
 AFSSLPinningModeNone,
 AFSSLPinningModePublicKey,
 AFSSLPinningModeCertificate,
 }

 `AFSSLPinningModeNone`
 Do not used pinned certificates to validate servers.

 `AFSSLPinningModePublicKey`
 Validate host certificates against public keys of pinned certificates.

 `AFSSLPinningModeCertificate`
 Validate host certificates against pinned certificates.
*/
