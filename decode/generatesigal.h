#ifndef GENERATESIGAL_H
#define GENERATESIGAL_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief 生成指定频率、幅度、通道数的正弦波信号数据（交错格式）
 * @tparam SampleType 信号样本的数据类型（float, double, int16_t, int32_t 等）
 * @param sampleRate   采样率（Hz）
 * @param frequency    信号频率（Hz）
 * @param amplitude    幅度（对于浮点类型为实际峰值；对于整型为归一化峰值，取值范围 [0,1]）
 * @param channels     通道数
 * @param durationSec  持续时间（秒），默认为 1.0
 * @return std::vector<SampleType> 交错存储的信号数据（长度为 sampleRate * durationSec * channels）
 * @throws std::invalid_argument 当参数无效时抛出
 */
template<typename SampleType>
std::vector<SampleType> generateSignal(double sampleRate,
                                       double frequency,
                                       double amplitude,
                                       int channels,
                                       double durationSec = 1.0){
    // 参数有效性检查
    if (sampleRate <= 0.0 || frequency < 0.0 || amplitude < 0.0 || channels <= 0 || durationSec <= 0.0) {
        throw std::invalid_argument("Invalid signal parameters");
    }
    if (frequency > sampleRate / 2.0) {
        // 警告：不满足奈奎斯特定理，但允许生成（注意混叠）
        // 实际使用时可根据需要抛出异常或仅打印警告
    }

    // 计算总样本数（每个通道）
    size_t numSamplesPerChannel = static_cast<size_t>(sampleRate * durationSec);
    // 交错数组总长度
    size_t totalSamples = numSamplesPerChannel * channels;
    std::vector<SampleType> signal;
    signal.reserve(totalSamples);

    // 对于整型，需要将幅度映射到整型范围；对于浮点型直接使用 amplitude
    constexpr bool isInteger = std::is_integral_v<SampleType>;
    double scaleFactor = 1.0;
    SampleType maxVal = 0;
    SampleType minVal = 0;
    if constexpr (isInteger) {
        // 幅度被视为归一化因子 [0,1]，乘以整型的最大值
        if (amplitude > 1.0) {
            throw std::invalid_argument("Amplitude for integer types must be in [0,1]");
        }
        maxVal = std::numeric_limits<SampleType>::max();
        minVal = std::numeric_limits<SampleType>::min();
        scaleFactor = amplitude * maxVal;
    }

    const double angularFreq = 2.0 * M_PI * frequency;
    const double invSampleRate = 1.0 / sampleRate;

    // 生成交错数据：对每个采样点，先计算正弦值，再为每个通道填入相同的值
    for (size_t i = 0; i < numSamplesPerChannel; ++i) {
        double t = i * invSampleRate;
        double value = std::sin(angularFreq * t);   // 范围 [-1, 1]
        double scaled = amplitude * value;          // 浮点型实际值

        if constexpr (isInteger) {
            // 整型：缩放并饱和转换
            double intVal = scaled * maxVal;
            SampleType sample;
            if (intVal >= static_cast<double>(maxVal)) {
                sample = maxVal;
            } else if (intVal <= static_cast<double>(minVal)) {
                sample = minVal;
            } else {
                sample = static_cast<SampleType>(std::llround(intVal));
            }
            // 每个通道填入相同值
            for (int ch = 0; ch < channels; ++ch) {
                signal.push_back(sample);
            }
        } else {
            // 浮点型：直接使用 scaled，然后填入每个通道
            SampleType sample = static_cast<SampleType>(scaled);
            for (int ch = 0; ch < channels; ++ch) {
                signal.push_back(sample);
            }
        }
    }

    return signal;
}

/**
 * @brief 连续信号生成器（正弦波），相位连续，可无限生成样本
 * @tparam SampleType 样本数据类型（float, double, int16_t, int32_t...）
 */
template<typename SampleType>
class ContinuousSignalGenerator {
public:
    /**
     * @param sampleRate   采样率（Hz）>0
     * @param frequency    信号频率（Hz）>=0
     * @param amplitude    幅度（float/double类型为峰值；整型为归一化峰值 [0,1]）
     * @param channels     通道数 >0，所有通道输出相同信号
     * @throws std::invalid_argument 参数非法时抛出
     */
    ContinuousSignalGenerator(double sampleRate, double frequency, 
                              double amplitude, int channels)
    : sampleRate_(sampleRate), frequency_(frequency), amplitude_(amplitude),
      channels_(channels), phase_(0.0), invSampleRate_(1.0 / sampleRate) 
    {
        
        if (sampleRate_ <= 0.0 || frequency_ < 0.0 || amplitude_ < 0.0 || channels_ <= 0) {
            throw std::invalid_argument("Invalid parameters for ContinuousSignalGenerator");
        }
        
        // 幅度归一化检查（整型）
        constexpr bool isInteger = std::is_integral_v<SampleType>;
        if constexpr (isInteger) {
            if (amplitude_ > 1.0) {
                throw std::invalid_argument("Amplitude for integer types must be in [0,1]");
            }
            maxVal_ = std::numeric_limits<SampleType>::max();
            minVal_ = std::numeric_limits<SampleType>::min();
        } else {
            maxVal_ = 1.0; // 无实际作用
            minVal_ = -1.0;
        }
        
        // 预计算相位增量（每采样点增加的弧度）
        deltaPhase_ = 2.0 * M_PI * frequency_ * invSampleRate_;
    }
    /**
     * @brief 重置相位（使生成器重新从0相位开始）
     * @param initialPhase 初始相位（弧度），默认为0
     */
    void resetPhase(double initialPhase = 0.0) {
        phase_ = initialPhase;
    }
    
    /**
     * @brief 改变频率（保持相位连续性）
     */
    void setFrequency(double newFrequency) {
        frequency_ = newFrequency;
        deltaPhase_ = 2.0 * M_PI * frequency_ * invSampleRate_;
    }
    
    /**
     * @brief 改变幅度（不影响相位）
     */
    void setAmplitude(double newAmplitude) {
        amplitude_ = newAmplitude;
    }
    
    /**
     * @brief 生成单个样本值（单通道）
     * @return SampleType 当前样本值
     */
    SampleType nextSample() {
        // 计算正弦值
        double value = std::sin(phase_);
        // 更新相位（保持范围在 [0, 2π) 避免累积误差）
        phase_ += deltaPhase_;
        if (phase_ >= 2.0 * M_PI) {
            phase_ -= 2.0 * M_PI;
        }
        
        // 缩放幅度
        double scaled = amplitude_ * value;
        
        // 转换为目标数据类型
        return convertSample(scaled);
    }
    
    /**
     * @brief 生成一个多通道帧（交错格式）的所有通道样本
     * @return std::vector<SampleType> 长度为 channels_，存放当前帧各通道值
     */
    std::vector<SampleType> nextFrame() {
        SampleType single = nextSample();  // 同一时刻所有通道相同
        return std::vector<SampleType>(channels_, single);
    }
    
    /**
     * @brief 生成一批交错格式的连续样本
     * @param numSamples 要生成的样本总数（每个通道计数，即交错数组长度）
     * @return std::vector<SampleType> 长度为 numSamples 的交错数据
     */
    std::vector<SampleType> generateBlock(size_t numSamples) {
        std::vector<SampleType> block;
        block.reserve(numSamples);
        for (size_t i = 0; i < numSamples; ++i) {
            SampleType s = nextSample();
            for (int ch = 0; ch < channels_; ++ch) {
                block.push_back(s);
            }
        }
        return block;
    }
    
    // 获取当前相位（弧度）
    double getPhase() const { return phase_; }
    int getChannels() const { return channels_; }
    double getSampleRate() const { return sampleRate_; }
    double getFrequency() const { return frequency_; }
    
private:
    SampleType convertSample(double scaledValue){
        constexpr bool isInteger = std::is_integral_v<SampleType>;
        if constexpr (isInteger) {
            // 整型缩放：scaledValue 范围 [-amplitude, amplitude]，映射到整型范围
            double intVal = (scaledValue / amplitude_) * maxVal_;  // 注意 amplitude>0
            intVal = std::clamp(intVal, static_cast<double>(minVal_), static_cast<double>(maxVal_));
            return static_cast<SampleType>(std::llround(intVal));
        } else {
            // 浮点型直接返回，范围在 [-amplitude, amplitude]
            return static_cast<SampleType>(scaledValue);
        }
    }
    
    double sampleRate_;
    double frequency_;
    double amplitude_;
    int channels_;
    double phase_;          // 当前相位（弧度）
    double deltaPhase_;     // 每采样点相位增量
    double invSampleRate_;
    
    // 整型转换辅助成员
    double maxVal_;
    double minVal_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

// 支持的波形类型
enum class WaveformType {
    Sine,       // 正弦波
    Square,     // 方波（占空比50%）
    Triangle,   // 三角波
    Sawtooth,   // 锯齿波（上升锯齿）
    Noise       // 白噪声（均匀分布）
};

/**
 * @brief 多功能波形生成器（连续生成，相位连续）
 * @tparam SampleType 样本数据类型（float, double, int16_t, int32_t...）
 */
template<typename SampleType>
class WaveformGenerator {
public:
    /**
     * @param sampleRate   采样率（Hz）>0
     * @param frequency    信号频率（Hz）>=0（噪声时忽略）
     * @param amplitude    幅度（浮点类型为峰值；整型为归一化峰值 [0,1]）
     * @param channels     通道数 >0，所有通道输出相同波形
     * @param type         波形类型，默认为正弦波
     * @throws std::invalid_argument 参数非法时抛出
     */
    WaveformGenerator(double sampleRate, double frequency, double amplitude,
                      int channels, WaveformType type = WaveformType::Sine)
        : sampleRate_(sampleRate), frequency_(frequency), amplitude_(amplitude),
          channels_(channels), type_(type), phase_(0.0), invSampleRate_(1.0 / sampleRate) {
        
        if (sampleRate_ <= 0.0 || frequency_ < 0.0 || amplitude_ < 0.0 || channels_ <= 0) {
            throw std::invalid_argument("Invalid parameters for WaveformGenerator");
        }
        
        constexpr bool isInteger = std::is_integral_v<SampleType>;
        if constexpr (isInteger) {
            if (amplitude_ > 1.0) {
                throw std::invalid_argument("Amplitude for integer types must be in [0,1]");
            }
            maxVal_ = std::numeric_limits<SampleType>::max();
            minVal_ = std::numeric_limits<SampleType>::min();
        }
        
        // 预计算相位增量
        updateDeltaPhase();
        
        // 噪声生成器初始化
        if (type_ == WaveformType::Noise) {
            noiseGen_ = std::mt19937(rd_());
            noiseDist_ = std::uniform_real_distribution<double>(-1.0, 1.0);
        }
    }
    
    /**
     * @brief 改变波形类型（相位重置可选）
     * @param type      新波形类型
     * @param resetPhase 是否重置相位（默认false，保持连续性）
     */
    void setWaveformType(WaveformType type, bool resetPhase = false) {
        type_ = type;
        if (resetPhase) phase_ = 0.0;
    }
    
    /**
     * @brief 改变频率（保持相位连续性）
     */
    void setFrequency(double newFrequency) {
        frequency_ = newFrequency;
        updateDeltaPhase();
    }
    
    /**
     * @brief 改变幅度
     */
    void setAmplitude(double newAmplitude) {
        amplitude_ = newAmplitude;
    }
    
    /**
     * @brief 重置相位到指定弧度
     */
    void resetPhase(double initialPhase = 0.0) {
        phase_ = initialPhase;
    }
    
    /**
     * @brief 生成单个样本值（单通道）
     */
    SampleType nextSample() {
        double value = 0.0;
        
        if (type_ == WaveformType::Noise) {
            value = noiseDist_(noiseGen_);
        } else {
            // 计算归一化波形值 [-1, 1]
            double normPhase = phase_ / (2.0 * M_PI);  // [0, 1)
            value = computeWaveformValue(normPhase);
            // 更新相位
            phase_ += deltaPhase_;
            if (phase_ >= 2.0 * M_PI) {
                phase_ -= 2.0 * M_PI;
            }
        }
        
        // 缩放幅度并转换数据类型
        double scaled = amplitude_ * value;
        return convertSample(scaled);
    }
    
    /**
     * @brief 生成一个多通道帧（所有通道值相同）
     * @return std::vector<SampleType> 长度为 channels_
     */
    std::vector<SampleType> nextFrame() {
        SampleType single = nextSample();
        return std::vector<SampleType>(channels_, single);
    }
    
    /**
     * @brief 生成一批交错格式的连续样本
     * @param numSamples 要生成的样本总数（每个通道计数，即交错数组长度）
     * @return std::vector<SampleType> 长度为 numSamples 的交错数据
     */
    std::vector<SampleType> generateBlock(size_t numSamples) {
        std::vector<SampleType> block;
        block.reserve(numSamples);
        for (size_t i = 0; i < numSamples; ++i) {
            SampleType s = nextSample();
            for (int ch = 0; ch < channels_; ++ch) {
                block.push_back(s);
            }
        }
        return block;
    }
    
    // 查询当前状态
    double getPhase() const { return phase_; }
    int getChannels() const { return channels_; }
    double getSampleRate() const { return sampleRate_; }
    double getFrequency() const { return frequency_; }
    WaveformType getWaveformType() const { return type_; }

private:
    /**
     * @brief 根据归一化相位 [0,1) 计算波形幅度 [-1,1]
     */
    double computeWaveformValue(double phaseNorm) {
        switch (type_) {
            case WaveformType::Sine:
                return std::sin(2.0 * M_PI * phaseNorm);
                
            case WaveformType::Square:
                // 方波：相位 < 0.5 时为 +1，否则 -1
                return (phaseNorm < 0.5) ? 1.0 : -1.0;
                
            case WaveformType::Triangle:
                // 三角波：在 [0,0.5) 上升，[0.5,1) 下降
                if (phaseNorm < 0.25) {
                    return 4.0 * phaseNorm;               // 0 -> 1
                } else if (phaseNorm < 0.75) {
                    return 2.0 - 4.0 * phaseNorm;         // 1 -> -1
                } else {
                    return -4.0 + 4.0 * phaseNorm;        // -1 -> 0
                }
                
            case WaveformType::Sawtooth:
                // 锯齿波：从 -1 线性增长到 +1
                return 2.0 * phaseNorm - 1.0;
                
            case WaveformType::Noise:
                // 噪声不在此分支处理，因为不需要相位更新
                return 0.0; // never reached
                
            default:
                return 0.0;
        }
    }
    
    void updateDeltaPhase() {
        deltaPhase_ = 2.0 * M_PI * frequency_ * invSampleRate_;
    }
    
    SampleType convertSample(double scaledValue) {
        constexpr bool isInteger = std::is_integral_v<SampleType>;
        if constexpr (isInteger) {
            // 整型：将范围 [-amplitude, amplitude] 映射到整型范围
            // 注意 amplitude>0，且 scaledValue 范围已在 [-amplitude, amplitude]
            double intVal = (scaledValue / amplitude_) * maxVal_;
            intVal = std::clamp(intVal, static_cast<double>(minVal_), static_cast<double>(maxVal_));
            return static_cast<SampleType>(std::llround(intVal));
        } else {
            // 浮点直接返回
            return static_cast<SampleType>(scaledValue);
        }
    }
    
    double sampleRate_;
    double frequency_;
    double amplitude_;
    int channels_;
    WaveformType type_;
    double phase_;          // 当前相位（弧度）
    double deltaPhase_;     // 每采样点相位增量
    double invSampleRate_;
    
    // 整型转换辅助
    double maxVal_;
    double minVal_;
    
    // 噪声生成器（仅当类型为 Noise 时使用）
    std::random_device rd_;
    std::mt19937 noiseGen_;
    std::uniform_real_distribution<double> noiseDist_;
};

#endif 