#include "Metal/Metal.hpp"

#include <cstddef>
#include <iostream>

constexpr size_t NUM_ELEMENTS = 10;
constexpr size_t BUFFER_SIZE = NUM_ELEMENTS * sizeof(float);

auto
fillRand(MTL::Buffer* buf) -> void
{
    int* dataPtr = static_cast<int*>(buf->contents());

    for (size_t i{0}; i < NUM_ELEMENTS; i++)
    {
        dataPtr[i] = rand() % 50;
    }
}

auto
printBuffer(MTL::Buffer* buf) -> void
{
    int* dataPtr = static_cast<int*>(buf->contents());

    for (size_t i{0}; i < NUM_ELEMENTS; i++) {
        std::cout << dataPtr[i] << ' ';
    }
    std::cout << '\n';
}

auto
main() -> int
{
    srand(time(NULL));

    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    MTL::Buffer* bufA =
        device->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    MTL::Buffer* bufB =
        device->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    MTL::Buffer* bufRes =
        device->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);

    NS::Error* error = NS::Error::alloc()->init();
    MTL::Library* defLib = device->newDefaultLibrary();
    assert(defLib &&  "Failed to initialise default library");

    NS::String* name = NS::String::alloc()->init(
        "add_matrices", NS::StringEncoding::ASCIIStringEncoding);
    MTL::Function* func = defLib->newFunction(name);
    assert(func && "Failed to initialise MATMUL func");

    MTL::ComputePipelineState* state =
        device->newComputePipelineState(func, &error);

    MTL::CommandQueue* queue = device->newCommandQueue();

    MTL::CommandBuffer* cmd_buf = queue->commandBuffer();
    MTL::ComputeCommandEncoder* enc = cmd_buf->computeCommandEncoder();

    fillRand(bufA);
    fillRand(bufB);
    printBuffer(bufA);
    printBuffer(bufB);

    enc->setComputePipelineState(state);
    enc->setBuffer(bufA, 0, 0);
    enc->setBuffer(bufB, 0, 1);
    enc->setBuffer(bufRes, 0, 2);

    MTL::Size grid_sz = MTL::Size(BUFFER_SIZE, 1, 1);
    MTL::Size warp_sz = MTL::Size(BUFFER_SIZE, 1, 1);
    enc->dispatchThreads(grid_sz, warp_sz);
    enc->endEncoding();
    cmd_buf->commit();
    cmd_buf->waitUntilCompleted();

    printBuffer(bufRes);
    std::cout << "\n";

    enc->setBuffer(bufA, 0, 0);
    enc->setBuffer(bufB, 0, 1);
    enc->setBuffer(bufRes, 0, 2);

    enc->dispatchThreads(grid_sz, warp_sz);
    enc->endEncoding();
    cmd_buf->commit();
    cmd_buf->waitUntilCompleted();

    printBuffer(bufRes);

    return -1;
}
