// === Funktionalität ===
// Primär: Live Töne/Sounds konfigurieren und abspielen
// Sekundär: Abspeichern als z.B. Wave-File

// === Architektur === -> DRAW.io
// - Einstellungen
// - SoundGenerator (generating sound information)
// - OutputStream

#include <cmath>
#include <iostream>
#include <string>
#include "portaudio.h"

constexpr double SAMPLE_RATE = 44100.0;
constexpr double FREQUENCY   = 440.0;
constexpr double DURATION    = 3.0;

int main()
{
    PaError err;

    // PortAudio initialisieren
    err = Pa_Initialize();
    if (err != paNoError)
    {
        std::cerr << "PortAudio Fehler: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    PaStream* stream;

    // Standard-Ausgabestream öffnen
    err = Pa_OpenDefaultStream(
        &stream,
        0,          // keine Eingabekanäle
        1,          // 1 Ausgabekanal (Mono)
        paFloat32,  // Sampleformat
        SAMPLE_RATE,
        256,        // Frames pro Buffer
        nullptr,    // keine Callback-Funktion
        nullptr
    );

    if (err != paNoError)
    {
        std::cerr << "Stream-Fehler: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    Pa_StartStream(stream);

    int totalSamples = static_cast<int>(SAMPLE_RATE * DURATION);

    for (int i = 0; i < totalSamples; i++)
    {
        float sample =
            static_cast<float>(0.2 * sin(2.0 * M_PI * FREQUENCY * i / SAMPLE_RATE));

        // einzelnes Sample schreiben
        err = Pa_WriteStream(stream, &sample, 1);

        if (err != paNoError)
        {
            std::cerr << "Write-Fehler: " << Pa_GetErrorText(err) << std::endl;
            break;
        }
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}