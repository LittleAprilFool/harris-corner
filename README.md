#Harris Corner

#### Harris Corner 角点检测

> 对输入的一张彩色图像，实现Harris Corner检
测算法

## 一、开发软件说明
### 开发环境
- Ubuntu 14.04
- cmake 2.8.12.2
- opencv 3.0

### 运行方式
- build project `make all`
- run `./harris_corner ./Resource/cv.png 0.02 4`

## 二、算法具体步骤
#### 2.1 根据原图计算每个点的Intensity
- 读入的图片位于RGB色彩空间，需要先将其转入HSI色彩空间
- RGB与HSI之间的转换公式如下

![](./pic/rgb2hsi.jpg)

#### 2.2 计算每个点的Ix, Iy
- 利用微分算子进行卷积，求出Ix, Iy
- Prewitt算子
	- 水平方向 ![](./pic/prewitt_x.jpg)
	- 垂直方向 ![](./pic/prewitt_y.jpg)
- Sobel算子
	- 水平方向 ![](./pic/sobel_x.jpg)
	- 垂直方向 ![](./pic/sobel_y.jpg)
	
#### 2.3 根据矩阵Ix, Iy计算矩阵M
- 矩阵M的计算公式

![](./pic/m.tiff)

#### 2.4 计算矩阵M的最大特征值Le和最小特征值Se
#### 2.5 根据矩阵Le和Se计算矩阵R
- 矩阵R的计算公式

![](./pic/r.tiff)

#### 2.6 过滤R值
- 设置一个阈值过滤R值
- 在局部区域内取R值最大的点就是检测出的角点

#### 2.7 将结果与原图混合输出

## 三、算法实现要点
#### 3.1 计算Intensity矩阵

![](./pic/31.tiff)

#### 3.2 计算Ix, Iy

![](./pic/32.tiff)

#### 3.3 计算矩阵M，Le，Se，R

![](./pic/33.tiff)

#### 3.4 过滤R值

![](./pic/34.tiff)

#### 3.5 混合输出

![](./pic/35.tiff)

## 四、实验结果展示及分析
#### 4.1 待检测的原图

![](./pic/cv.png)

#### 4.2 灰度图

![](./pic/intensity.jpg)

#### 4.3 Ix

![](./pic/ix.jpg)

#### 4.4 Iy

![](./pic/iy.jpg)

#### 4.5 Le

![](./pic/le.jpg)

#### 4.6 Se

![](./pic/se.jpg)

#### 4.7 R

![](./pic/r.jpg)

#### 4.8 过滤后的R

![](./pic/fr.jpg)

#### 4.9 叠加后

![](./pic/mix.jpg)
