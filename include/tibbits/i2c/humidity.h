/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LHUMIDITY_H__
#define __LHUMIDITY_H__

/*!
    \struct Hih6130
    \brief Struct with data output for HIH6130
*/

struct Hih6130
{
    float humidity; //< % RH
    float temperature; //< °C
    int status;
};

/*!
    \class Humidity
    \brief Class for ambient humidity/temperature meter tibbit control (Tibbit #30)
*/

class Humidity
{
public:

    Humidity();

    virtual ~Humidity();

    /// Get current humidity/temperature
    /*!
        \param bus I2C bus number
        \return Hih6130 data struct
    */
    Hih6130 getData(int bus);
};

#endif
