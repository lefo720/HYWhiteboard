//
//  HYConversationManager.h
//  HYEfficientWhiteBoard
//
//  Created by apple on 2017/10/24.
//  Copyright © 2017年 HaydenYe. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class HYSocketService, HYWbPoint;

#define kTimeIntervalSendCmd    0.06f   // 发送命令的时间间隔

#define kMsgPointFormatter      @"%zd,%f,%f,%d"  // 画点的命令:@"HYMessageCmd,point.x,point.y,type"
#define kMsgPenFormatter        @"%zd,%d,%d"     // 画笔的命令:@"HYMessageCmd,colorIndex,lineWidth"
#define kMsgEidtFormatter       @"%zd"           // 编辑的动作:@"HYMessageCmd"

typedef NS_ENUM(NSUInteger, HYMessageCmd) {
    HYMessageCmdNone = 0,           // 占位
    HYMessageCmdDrawPoint = 1,      // 画线
    HYMessageCmdEraserPoint = 2,    // 橡皮
    HYMessageCmdPenStyle = 3,       // 画笔样式
    HYMessageCmdCancel = 4,         // 撤销
    HYMessageCmdResume = 5,         // 恢复
    HYMessageCmdClearAll = 6,       // 清除所有画线
};


@protocol HYConversationDelegate <NSObject>

@optional

/**
 接收到画点消息
 
 @param point       画线的点
 @param type        画线的点的类型
 @param isEraser    是否为橡皮擦
 */
- (void)onReceivePoint:(CGPoint)point
                  type:(uint8_t)type
              isEraser:(BOOL)isEraser;


/**
 接收到画笔样式
 
 @param colorIndex  画笔的颜色的索引
 @param lineWidth   画笔的粗细
 */
- (void)onReceivePenColor:(NSInteger)colorIndex
                lineWidth:(NSInteger)lineWidth;


/**
 接收到撤销，恢复，全部擦除消息
 
 @param type 撤销，恢复，全部擦除
 */
- (void)onReceiveEditAction:(HYMessageCmd)type;

@end


@interface HYConversationManager : NSObject

@property (nonatomic, strong)NSMutableArray                 *cmdBuff;
@property (nonatomic, weak)id<HYConversationDelegate>       converDelegate;

@property (nonatomic, strong, readonly)HYSocketService      *serveice;
@property (nonatomic, assign, readonly)BOOL                 isConnected;


+ (instancetype)shared;


/**
 连接服务器
 
 @param host        服务器地址
 @param port        服务器端口号
 @param successd    连接成功
 @paraHYSocketService接失败
 */
- (void)connectWhiteboardServer:(NSString *)host
                           port:(int)port
                      successed:(void(^)(HYSocketService *service))successd
                         failed:(void(^)(NSError *error))failed;


/**
 发送画点的消息

 @param point       画点的信息
 */
- (void)sendPointMsg:(HYWbPoint *)point;


/**
 发送画笔样式

 @param colorIndex  画笔颜色索引
 @param lineWidth   画笔粗细
 */
- (void)sendPenStyleColor:(uint8_t)colorIndex
                lineWidth:(uint8_t)lineWidth;


/**
 发送撤销，恢复，清除所有

 @param action 撤销，恢复，清除所有
 */
- (void)sendEditAction:(HYMessageCmd)action;


/**
 断开会话的socket连接
 */
- (void)disconnectWhiteboard;

@end
