#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

#include "Interface.hpp"

class PricerAbstract
{
public:
    using PayoffFunc = std::function<double(double)>;
    using DiscounterFunc = std::function<double()>;

public: 
    PricerAbstract(const PayoffFunc& callpayoff, const PayoffFunc& putpayoff, const DiscounterFunc& discounter, std::size_t nSim)
        : m_callPayoff{ callpayoff }, m_putPayoff{ putpayoff }, m_discounter{discounter}, m_putPrice{}, m_callPrice{}, 
        m_callSum{}, m_putSum{}, m_NSim{nSim}
    {}
    virtual ~PricerAbstract() = default;

    DiscounterFunc discount_factor() const { return m_discounter; }         // Discounting
    virtual double call_price() const { return m_callPrice; }               // Call price
    virtual double put_price() const { return m_putPrice; }                 // Put price

    virtual void process_path(const std::vector<double>& path) = 0;         // Create a single path
    virtual void post_process(double duration) = 0;                         // Notify end of simulation

protected:
    PayoffFunc m_callPayoff;
    PayoffFunc m_putPayoff;
    DiscounterFunc m_discounter;
    double m_callPrice;
    double m_putPrice;
    double m_callSum;
    double m_putSum;
    std::atomic_size_t m_NSim; // Atomic to prevent data races
    std::mutex m_mutex;
};