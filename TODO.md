# todo

-> 写 回调
->  给线程写回调 OK
传出 (uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height

-> buffer转 qimage OK
QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGBA8888);
                QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
              
-> 播放器状态
取流, OK 待优化
录像


-> 写 c11 线程 ok
-> 把 frame 写到主界面 ，布局 OK

-> 修复显示bug OK 

-> 录像的状态
-> 多路录像
-> 录像命名
-> 多路复用 Ok



-> 写 test


