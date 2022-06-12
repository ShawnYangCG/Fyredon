set -ex
# models
RESULTS_DIR='./results/10color2earth'
G_PATH='./pretrained_models/10color2earth_net_G.pth'
E_PATH='./pretrained_models/10color2earth_net_E.pth'

# dataset
CLASS='10color2earth'
DIRECTION='AtoB' # from domain A to domain B
LOAD_SIZE=512 # scale images to this size
CROP_SIZE=512 # then crop to this size
INPUT_NC=1  # number of channels in the input image

# misc
GPU_ID=0   # gpu id
NUM_TEST=20 # number of input images duirng test
NUM_SAMPLES=20 # number of samples per input images

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
