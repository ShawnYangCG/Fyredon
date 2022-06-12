set -ex
# models
RESULTS_DIR='./results/10color2coloredtexture2'
G_PATH='./pretrained_models/10color2coloredtexture2_net_G.pth'
E_PATH='./pretrained_models/10color2coloredtexture2_net_E.pth'

# dataset
CLASS='10color2coloredtexture2'
DIRECTION='AtoB' # from domain A to domain B
LOAD_SIZE=1024 # scale images to this size
CROP_SIZE=1024 # then crop to this size
INPUT_NC=3 # number of channels in the input image

# misc
GPU_ID=0   # gpu id
NUM_TEST=20 # number of input images duirng test
NUM_SAMPLES=50 # number of samples per input images

# command
CUDA_VISIBLE_DEVICES=${GPU_ID} python ./test.py \
  --dataroot ./datasets/${CLASS} \
  --results_dir ${RESULTS_DIR} \
  --checkpoints_dir ./checkpoints/${CLASS} \
  --name ${CLASS} \
  --direction ${DIRECTION} \
  --load_size ${LOAD_SIZE} \
  --crop_size ${CROP_SIZE} \
  --input_nc ${INPUT_NC} \
  --num_test ${NUM_TEST} \
  --n_samples ${NUM_SAMPLES} \
  --center_crop \
  --no_flip \
  --no_encode
