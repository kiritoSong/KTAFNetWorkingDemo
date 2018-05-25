// AFURLResponseSerialization.h
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
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

/**
    AFURLResponseSerialization协议、同时需要遵循NSSecureCoding, NSCopying两个协议
 */
@protocol AFURLResponseSerialization <NSObject, NSSecureCoding, NSCopying>


/**
 
 必须实现的方法
 将响应体进行指定方式解码

 @param response 需要处理的`NSURLResponse`
 @param data 需要处理的数据
 @param error 错误

 @return 返回一个特定格式(Dic/Arr/Str/XML/Plist/图片等)
 
 比如AFURLSessionManager中任务结束后对data的转码时就这样使用:
 responseObject = [manager.responseSerializer responseObjectForResponse:task.response data:data error:&serializationError];
 
 其中manager.responseSerializer为`AFJSONRequestSerializer`或者`AFPropertyListRequestSerializer`、均为`AFHTTPRequestSerializer(遵循AFURLResponseSerialization协议)`的子类。
 */
- (nullable id)responseObjectForResponse:(nullable NSURLResponse *)response
                           data:(nullable NSData *)data
                          error:(NSError * _Nullable __autoreleasing *)error NS_SWIFT_NOTHROW;

@end

#pragma mark -

/**
 `AFHTTPResponseSerializer` conforms to the `AFURLRequestSerialization` & `AFURLResponseSerialization` protocols, offering a concrete base implementation of query string / URL form-encoded parameter serialization and default request headers, as well as response status code and content type validation.

 Any request or response serializer dealing with HTTP is encouraged to subclass `AFHTTPResponseSerializer` in order to ensure consistent default behavior.
 */
@interface AFHTTPResponseSerializer : NSObject <AFURLResponseSerialization>

//初始化
- (instancetype)init;

/**
    解码器的编码方式
 */
@property (nonatomic, assign) NSStringEncoding stringEncoding;

/**
    初始化
 */
+ (instancetype)serializer;

///-----------------------------------------
/// @name 配置响应解码器
///-----------------------------------------

/**
 接受并解析的HTTP状态码。如果不为nil、未包含的状态码将不被解析

 See http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
 */
@property (nonatomic, copy, nullable) NSIndexSet *acceptableStatusCodes;

/**
 接受并解析的Content-Type。如果不为nil、未包含的Content-Type将不被解析
 */
@property (nonatomic, copy, nullable) NSSet <NSString *> *acceptableContentTypes;

/**
 检测响应能否被解析
 
 @param response 响应
 @param data 二进制文件
 @param error 错误
 @return 能否被解析
 */
- (BOOL)validateResponse:(nullable NSHTTPURLResponse *)response
                    data:(nullable NSData *)data
                   error:(NSError * _Nullable __autoreleasing *)error;

@end

#pragma mark -


/**
 Json序列化

 默认接收一下几种类型的content-type

 - `application/json`
 - `text/json`
 - `text/javascript`
 */
@interface AFJSONResponseSerializer : AFHTTPResponseSerializer
//初始化
- (instancetype)init;

/**
    读取JSON文件的选项 默认 0
 
     typedef NS_OPTIONS(NSUInteger, NSJSONReadingOptions) {
     NSJSONReadingMutableContainers = (1UL << 0),//返回一个MDic/MArr
     NSJSONReadingMutableLeaves = (1UL << 1),//返回一个MStr
     NSJSONReadingAllowFragments = (1UL << 2)//允许解析最外层不是Dic或者Arr的Json、比如@"123"
     } API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
 
 */
@property (nonatomic, assign) NSJSONReadingOptions readingOptions;

/**
 是否屏蔽NSNULL、默认为NO
 */
@property (nonatomic, assign) BOOL removesKeysWithNullValues;

/**
 根据指定策略创建一个实例
 */
+ (instancetype)serializerWithReadingOptions:(NSJSONReadingOptions)readingOptions;

@end

#pragma mark -

/**
 XML序列化

 默认接收一下几种类型的content-type:

 - `application/xml`
 - `text/xml`
 */
@interface AFXMLParserResponseSerializer : AFHTTPResponseSerializer

@end

#pragma mark -

#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED

/**
 也是解析XML
 这个子类只在mac os x上使用

 - `application/xml`
 - `text/xml`
 */
@interface AFXMLDocumentResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
 Input and output options specifically intended for `NSXMLDocument` objects. For possible values, see the `NSJSONSerialization` documentation section "NSJSONReadingOptions". `0` by default.
 */
@property (nonatomic, assign) NSUInteger options;

/**
 Creates and returns an XML document serializer with the specified options.

 @param mask The XML document options.
 */
+ (instancetype)serializerWithXMLDocumentOptions:(NSUInteger)mask;

@end

#endif

#pragma mark -

/**
 PList序列化

 支持以下MIME types：
 - `application/x-plist`
 */
@interface AFPropertyListResponseSerializer : AFHTTPResponseSerializer

- (instancetype)init;

/**
 PList 格式
 typedef NS_ENUM(NSUInteger, NSPropertyListFormat) {
 NSPropertyListOpenStepFormat = kCFPropertyListOpenStepFormat,
 //指定属性列表文件格式为XML格式，仍然是纯文本类型，不会压缩文件
 NSPropertyListXMLFormat_v1_0 = kCFPropertyListXMLFormat_v1_0,
 //指定属性列表文件格式为二进制格式，文件是二进制类型，会压缩文件
 NSPropertyListBinaryFormat_v1_0 = kCFPropertyListBinaryFormat_v1_0
 //指定属性列表文件格式为ASCII码格式，对于旧格式的属性列表文件，不支持写入操作 
 };
 */
@property (nonatomic, assign) NSPropertyListFormat format;

/**
 PList 读取选项
 };
 */
@property (nonatomic, assign) NSPropertyListReadOptions readOptions;

/**
 Creates and returns a property list serializer with a specified format, read options, and write options.

 @param format The property list format.
 @param readOptions The property list reading options.
 */
+ (instancetype)serializerWithFormat:(NSPropertyListFormat)format
                         readOptions:(NSPropertyListReadOptions)readOptions;

@end

#pragma mark -

/**
 图像格式化

 MIME types:
 - `image/tiff`
 - `image/jpeg`
 - `image/gif`
 - `image/png`
 - `image/ico`
 - `image/x-icon`
 - `image/bmp`
 - `image/x-bmp`
 - `image/x-xbitmap`
 - `image/x-win-bitmap`
 */
@interface AFImageResponseSerializer : AFHTTPResponseSerializer

#if TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH
/**
 图片比例
 */
@property (nonatomic, assign) CGFloat imageScale;

/**
 是否在AFN线程解压图片
 当使用`setCompletionBlockWithSuccess:failure:`时、这个选项可以显著的提高性能
 默认YES
 */
@property (nonatomic, assign) BOOL automaticallyInflatesResponseImage;
#endif

@end

#pragma mark -

/**
 可以包含多个解析器的复合解析器
 */
@interface AFCompoundResponseSerializer : AFHTTPResponseSerializer

/**
 解析器数组
 */
@property (readonly, nonatomic, copy) NSArray <id<AFURLResponseSerialization>> *responseSerializers;

/**
 初始化
 */
+ (instancetype)compoundSerializerWithResponseSerializers:(NSArray <id<AFURLResponseSerialization>> *)responseSerializers;

@end

///----------------
/// @name Constants
///----------------

/**
 ## Error Domains

 The following error domain is predefined.

 - `NSString * const AFURLResponseSerializationErrorDomain`

 ### Constants

 `AFURLResponseSerializationErrorDomain`
 AFURLResponseSerializer errors. Error codes for `AFURLResponseSerializationErrorDomain` correspond to codes in `NSURLErrorDomain`.
 */
FOUNDATION_EXPORT NSString * const AFURLResponseSerializationErrorDomain;

/**
 ## User info dictionary keys

 These keys may exist in the user info dictionary, in addition to those defined for NSError.

 - `NSString * const AFNetworkingOperationFailingURLResponseErrorKey`
 - `NSString * const AFNetworkingOperationFailingURLResponseDataErrorKey`

 ### Constants

 `AFNetworkingOperationFailingURLResponseErrorKey`
 The corresponding value is an `NSURLResponse` containing the response of the operation associated with an error. This key is only present in the `AFURLResponseSerializationErrorDomain`.

 `AFNetworkingOperationFailingURLResponseDataErrorKey`
 The corresponding value is an `NSData` containing the original data of the operation associated with an error. This key is only present in the `AFURLResponseSerializationErrorDomain`.
 */
FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLResponseErrorKey;

FOUNDATION_EXPORT NSString * const AFNetworkingOperationFailingURLResponseDataErrorKey;

NS_ASSUME_NONNULL_END
