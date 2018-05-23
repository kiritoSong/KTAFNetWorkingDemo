//
//  ViewController.m
//  AFNetWorkingDemo
//
//  Created by 刘嵩野 on 2018/2/28.
//  Copyright © 2018年 kirito_song. All rights reserved.
//

#import "ViewController.h"
#import <AFNetworking.h>

#import <AssertMacros.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    
    /*
        .h文件
         AFHTTPRequestSerializer 负责请求的生产
         AFMultipartFormData 负责上传文件拼接的协议
         AFJSONRequestSerializer和AFPropertyListRequestSerializer:AFHTTPRequestSerializer的子类
         可以将参数转化成JSON上传.`Content-Type`分别为`application/JSON`和`application/x-plist`
         需要配置给AFURLSessionManager.requestSerializer/responseSerializer属性
     
        .m文件
         AFQueryStringPair 将字典的key:value逐个提取出来构造体
         AFStreamingMultipartFormData 用来整合请求体信息、并且整合进request
         AFMultipartBodyStream 请求体整合工具(可以将单个`AFHTTPBodyPart`追加)
         AFHTTPBodyPart 请求体单个片段(也就是单个name) 
     */

    
    AFHTTPRequestSerializer * requestSerializer =  [AFHTTPRequestSerializer serializer];
    [requestSerializer setValue:@"请求头value1" forHTTPHeaderField:@"请求头key1"];
    [requestSerializer setValue:@"请求头value2" forHTTPHeaderField:@"请求头key2"];

    NSMutableURLRequest * req = [requestSerializer requestWithMethod:@"POST" URLString:@"http://127.0.0.1:3000/" parameters:@{@"key":@"value"} error:nil];

    [[[AFHTTPSessionManager manager] dataTaskWithRequest:req completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
        
    }] resume] ;
    
    
    
}


- (void)RequireTest {
    //断言为假则会执行一下第三个action、抛出异常、并且跳到_out
    __Require_Action(1, _out, NSLog(@"直接跳"));
    __Require_Quiet(1,_out);
    NSLog(@"111");
    
    //如果不注释、从这里直接就会跳到out
    //    __Require_Quiet(0,_out);
    //    NSLog(@"222");
    
    //如果没有错误、也就是NO、继续执行
    __Require_noErr(NO, _out);
    NSLog(@"333");
    
    //如果有错误、也就是YES、跳到_out、并且抛出异常定位
    __Require_noErr(YES, _out);
    NSLog(@"444");
_out:
    NSLog(@"end");
    

}

+(NSSet<NSString *> *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
    
    return [super keyPathsForValuesAffectingValueForKey:key];
}





//- (void)URLSession:(NSURLSession *)session didBecomeInvalidWithError:(nullable NSError *)error {
//
//}
//
//- (void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge
// completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler {
//
//}
//
//- (void)URLSessionDidFinishEventsForBackgroundURLSession:(NSURLSession *)session {
//
//}


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error {
    
}



- (void)NSURLSessionConfigurationTest {

    NSURLSessionConfiguration * configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    AFURLSessionManager * manager = [[AFURLSessionManager alloc]initWithSessionConfiguration:configuration];
    
    
    
    
    //如果不添加反序列化、有可能会报错说传回来的res是text/html。
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    
    [manager setSessionDidReceiveAuthenticationChallengeBlock:^NSURLSessionAuthChallengeDisposition(NSURLSession * _Nonnull session, NSURLAuthenticationChallenge * _Nonnull challenge, NSURLCredential *__autoreleasing  _Nullable * _Nullable credential) {
        
        return NSURLSessionAuthChallengePerformDefaultHandling;
    }];
    
    NSURLRequest * request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://127.0.0.1:3000/"]];

    NSURLSessionDataTask * task = [manager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
        
        
    }];
    
    [task resume];
    
    [[manager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
        
        
    }] resume];
    

    
    
    
}

//多文件上传
- (void)uploadFileUseFormWithStreamed {
    
    NSDictionary *parameter = @{@"username": @"Mike"};
    AFURLSessionManager * manager = [[AFURLSessionManager alloc]init];
    

    NSMutableURLRequest * request = [[AFHTTPRequestSerializer serializer] multipartFormRequestWithMethod:@"POST" URLString:@"baidu" parameters:parameter constructingBodyWithBlock:^(id<AFMultipartFormData>  _Nonnull formData) {
        
        NSString *documentFolder = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
        NSURL *fileURL = [NSURL fileURLWithPath:[documentFolder stringByAppendingPathComponent:@"1.txt"]];
        [formData appendPartWithFileURL:fileURL name:@"userfile[]" error:NULL];
        NSURL *fileURL1 = [NSURL fileURLWithPath:[documentFolder stringByAppendingPathComponent:@"2.jpg"]];
        [formData appendPartWithFileURL:fileURL1 name:@"userfile[]" fileName:@"aaa.jpg" mimeType:@"image/jpeg" error:NULL];
        
    } error:nil];
    


    
    [[manager uploadTaskWithStreamedRequest:request progress:^(NSProgress * _Nonnull uploadProgress) {
        
    } completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
        
    }] resume];
    
    
    
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    
    [mgr POST:@"baidu" parameters:parameter constructingBodyWithBlock:^(id<AFMultipartFormData>  _Nonnull formData) {
        
        
        NSString *documentFolder = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
        NSURL *fileURL = [NSURL fileURLWithPath:[documentFolder stringByAppendingPathComponent:@"1.txt"]];
        [formData appendPartWithFileURL:fileURL name:@"userfile[]" error:NULL];
        NSURL *fileURL1 = [NSURL fileURLWithPath:[documentFolder stringByAppendingPathComponent:@"2.jpg"]];
        [formData appendPartWithFileURL:fileURL1 name:@"userfile[]" fileName:@"aaa.jpg" mimeType:@"image/jpeg" error:NULL];
        
        
    } progress:^(NSProgress * _Nonnull uploadProgress) {
        
    } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        
    }];
}

- (void)downTest {
    
    NSString * urlString = @"http://127.0.0.1:3000/download/kirito##@#";

    AFURLSessionManager *manager = [[AFURLSessionManager alloc] init];
    
    NSMutableCharacterSet *mutableCharacterSet = [[NSMutableCharacterSet alloc] init];
    [mutableCharacterSet formUnionWithCharacterSet:[NSCharacterSet URLHostAllowedCharacterSet]];
    [mutableCharacterSet formUnionWithCharacterSet:[NSCharacterSet URLPathAllowedCharacterSet]];
    NSString *escapedURLString = [urlString stringByAddingPercentEncodingWithAllowedCharacters:mutableCharacterSet];
    
    NSURL *url = [NSURL URLWithString:escapedURLString];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    [[manager downloadTaskWithRequest:request progress:^(NSProgress * _Nonnull downloadProgress) {
        
        if (downloadProgress.fractionCompleted == 1) {
            NSLog(@"下载进度 100%%");
        }
        //        NSLog(@"已完成大小:%lld  总大小:%lld", downloadProgress.completedUnitCount, downloadProgress.totalUnitCount);
        //        NSLog(@"进度:%0.2f%%", downloadProgress.fractionCompleted * 100);
        
    } destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
        
        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
        /*
         kirito.实际文件后缀(mov)
         */
        NSString *fileName = httpResponse.suggestedFilename ?: urlString;
        /*
         /Users/kiritoSong/Library/Developer/CoreSimulator/Devices/49B05E71-2D4E-4A9E-A93C-89BE36EED962/data/Containers/Data/Application/E620C9F3-5DCD-45B3-9AE7-A6CB85ED50E8/Documents/kirito.mov
         */
        NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:fileName];
        /*
         file:///Users/kiritoSong/Library/Developer/CoreSimulator/Devices/49B05E71-2D4E-4A9E-A93C-89BE36EED962/data/Containers/Data/Application/69AE9F53-95E1-4855-B4AF-2BE767A4A341/Documents/video_01.mov
         */
        NSURL *destinationURL = [NSURL fileURLWithPath:filePath];
        NSLog(@"更改路径");
        return destinationURL;
        
    } completionHandler:^(NSURLResponse *response, NSURL *filePath, NSError *error) {
        if (error) {
            NSLog(@"%@", error);
        }else {
            NSLog(@"下载完成");
        }
        
    }] resume];
    
}



- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    if ([object isKindOfClass:[NSProgress class]]) {
        NSProgress *p = object;
        NSLog(@"已完成大小:%lld  总大小:%lld", p.completedUnitCount, p.totalUnitCount);
        NSLog(@"进度:%0.2f%%", p.fractionCompleted * 100);
    }
}
    



- (void)downloadWithURLString:(NSString *)urlString progress:(NSProgress * __autoreleasing *)progress completionHandler:(void (^)(NSURLResponse *response, NSURL *filePath, NSError *error))completionHandler {
    
    AFURLSessionManager *manager = [[AFURLSessionManager alloc] init];
    
    NSMutableCharacterSet *mutableCharacterSet = [[NSMutableCharacterSet alloc] init];
    [mutableCharacterSet formUnionWithCharacterSet:[NSCharacterSet URLHostAllowedCharacterSet]];
    [mutableCharacterSet formUnionWithCharacterSet:[NSCharacterSet URLPathAllowedCharacterSet]];
    NSString *escapedURLString = [urlString stringByAddingPercentEncodingWithAllowedCharacters:mutableCharacterSet];
    
    NSURL *url = [NSURL URLWithString:escapedURLString];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];

    [[manager downloadTaskWithRequest:request progress:^(NSProgress * _Nonnull downloadProgress) {
        
        if (downloadProgress.fractionCompleted == 1) {
            NSLog(@"下载进度 100%%");
        }
//        NSLog(@"已完成大小:%lld  总大小:%lld", downloadProgress.completedUnitCount, downloadProgress.totalUnitCount);
//        NSLog(@"进度:%0.2f%%", downloadProgress.fractionCompleted * 100);

    } destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
        
        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
        /*
           kirito.实际文件后缀(mov)
         */
        NSString *fileName = httpResponse.suggestedFilename ?: urlString;
        /*
         /Users/kiritoSong/Library/Developer/CoreSimulator/Devices/49B05E71-2D4E-4A9E-A93C-89BE36EED962/data/Containers/Data/Application/E620C9F3-5DCD-45B3-9AE7-A6CB85ED50E8/Documents/kirito.mov
         */
        NSString *filePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:fileName];
        /*
         file:///Users/kiritoSong/Library/Developer/CoreSimulator/Devices/49B05E71-2D4E-4A9E-A93C-89BE36EED962/data/Containers/Data/Application/69AE9F53-95E1-4855-B4AF-2BE767A4A341/Documents/video_01.mov
         */
        NSURL *destinationURL = [NSURL fileURLWithPath:filePath];
        NSLog(@"更改路径");
        return destinationURL;
        
    } completionHandler:^(NSURLResponse *response, NSURL *filePath, NSError *error) {
        if (error) {
            NSLog(@"%@", error);
        }else {
            NSLog(@"下载完成");
        }
        
    }] resume];
    
    
    
}



- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
