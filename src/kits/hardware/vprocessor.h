/*
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VPROCESSOR_H
#define VPROCESSOR_H

#include <VibeHardware/VDeviceInterface>

class VProcessorPrivate;
class VDevice;

/**
 * This device interface is available on processors.
 */
class VIBE_EXPORT VProcessor : public VDeviceInterface
{
    Q_OBJECT
    Q_ENUMS(InstructionSet)
    Q_FLAGS(InstructionSets)
    Q_PROPERTY(int number READ number)
    Q_PROPERTY(qulonglong maxSpeed READ maxSpeed)
    Q_PROPERTY(bool canChangeFrequency READ canChangeFrequency)
    Q_PROPERTY(InstructionSets instructionSets READ instructionSets)
    Q_DECLARE_PRIVATE(VProcessor)
    friend class VDevice;

private:
    /**
     * Creates a new VProcessor object.
     * You generally won't need this. It's created when necessary using
     * VDevice::as().
     *
     * @param backendObject the device interface object provided by the backend
     * @see VDevice::as()
     */
    explicit VProcessor(QObject *backendObject);

public:
    /**
     * This enum contains the list of architecture extensions you
     * can query.
     */
    enum InstructionSet {
        NoExtensions = 0x0,
        IntelMmx = 0x1,
        IntelSse = 0x2,
        IntelSse2 = 0x4,
        IntelSse3 = 0x8,
        IntelSse4 = 0x10,
        Amd3DNow = 0x20,
        AltiVec = 0x40
    };

    /*
     * The flags for the Extension enum.
     */
    Q_DECLARE_FLAGS(InstructionSets, InstructionSet)


    /**
     * Destroys a VProcessor object.
     */
    virtual ~VProcessor();

    /**
     * Get the VDeviceInterface::Type of the Processor device interface.
     *
     * @return the Processor device interface type
     * @see VHardware::Ifaces::Enums::DeviceInterface::Type
     */
    static Type deviceInterfaceType() {
        return VDeviceInterface::Processor;
    }

    /**
     * Retrieves the processor number in the system.
     *
     * @return the internal processor number in the system, starting from zero
     */
    int number() const;

    /**
     * Retrieves the maximum speed of the processor.
     *
     * @return the maximum speed in MHz, or 0 if the device can't be queried for this
     * information.
     */
    int maxSpeed() const;

    /**
     * Indicates if the processor can change the CPU frequency.
     *
     * True if a processor is able to change its own CPU frequency.
     *  (generally for power management).
     *
     * @return true if the processor can change CPU frequency, false otherwise
     */
    bool canChangeFrequency() const;

    /**
     * Queries the instructions set extensions of the CPU.
     *
     * @return the extensions supported by the CPU
     * @see VProcessor::InstructionSet
     */
    InstructionSets instructionSets() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VProcessor::InstructionSets)

#endif // VPROCESSOR_H