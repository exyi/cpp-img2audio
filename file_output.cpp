#include "my_std.hpp"

template<typename T>
void write_to_stream(ostream& stream, T value)
{
    stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

void save_waveform(const string& filename, const vector<f32>& buffer, i32 samplerate)
{
    ofstream stream;
    stream.open(filename, ios::out | ios::binary);

    int buffer_size = buffer.size();
    stream.write("RIFF", 4);                    // RIFF chunk
    write_to_stream(stream, (u32)(36 + buffer_size*sizeof(int))); // RIFF chunk size in bytes
    stream.write("WAVE", 4);                    // WAVE chunk
    stream.write("fmt ", 4);                    // fmt chunk
    write_to_stream(stream, (u32)16);                     // size of fmt chunk
    write_to_stream(stream, (u16)3);                       // Format = PCM
    write_to_stream(stream, (u16)1);                       // # of Channels
    write_to_stream(stream, (u32)samplerate);                // Sample Rate
    write_to_stream(stream, (u32)(samplerate*sizeof(int)));    // Byte rate
    write_to_stream(stream, (u16)sizeof(int));             // Frame size
    write_to_stream(stream, (u16)32);                      // Bits per sample
    stream.write("data", 4);                   // data chunk
    write_to_stream(stream, (u32)(buffer_size*sizeof(int)));   // data chunk size in bytes


    for (int i = 0; i < buffer_size; ++i) // Single cycle
    {
        f32 tmp = buffer[i];
        // double tmp = sin(((440.0 + ((f64)i / samplerate * 100)) * (2 * M_PI) * i) / (f64)samplerate) * 0.7;
        // int intVal = (int)(tmp * 2147483647.0) & 0xffffff00;
        // stream << intVal;
        // cout << tmp << "\n";
        // write_to_stream(stream, (i32)intVal);
        write_to_stream(stream, (f32)tmp);
    }

}
