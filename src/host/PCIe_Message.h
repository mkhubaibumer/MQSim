#ifndef PCIE_MESSAGE_H
#define PCIE_MESSAGE_H

#include<cstdint>
#include <sstream>

namespace Host_Components {
    enum class PCIe_Destination_Type {
        HOST, DEVICE
    };
    enum class PCIe_Message_Type {
        READ_REQ, WRITE_REQ, READ_COMP
    };

    class PCIe_Message {
    public:
        PCIe_Destination_Type Destination;
        PCIe_Message_Type Type;
        void *Payload;
        unsigned int Payload_size;
        uint64_t Address;

        [[nodiscard]] std::string str() const {
            std::ostringstream st;
            st << "**PCIe_Message** Destination: " << (int) Destination << " Type: " << (int) Type << " Payload Size: " << Payload_size
               << " Address: " << Address << "\n";
            return st.str();
        }
    };
}

#endif //!PCIE_MESSAGE_H
