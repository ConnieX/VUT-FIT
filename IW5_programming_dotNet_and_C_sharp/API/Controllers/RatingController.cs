using System.Linq;
using System.Threading.Tasks;
using API.Data;
using API.Models.RatingModels;
using AutoMapper;
using Microsoft.AspNetCore.Mvc;

namespace API.Controllers {
    [ApiController]
    [Route("api/[controller]")]
    public class RatingController {
        private readonly ApplicationDbContext _database;
        private readonly IMapper _mapper;

        public RatingController(ApplicationDbContext database, IMapper mapper) {
            _database = database;
            _mapper = mapper;
        }

        [HttpGet]
        public IActionResult GetAll() {
            return new JsonResult(_database.Ratings.Select(x => new RatingList(x)));
        }

        [HttpGet("{id}")]
        public IActionResult GetDetail(int id) {
            //control if rating exists, if yes, return detail of rating, if not, return error
            var item = _database.Ratings.Find(id);
            if (item == null) {
                return new NotFoundResult();
            }

            return new JsonResult(item);
        }

        [HttpPost]
        public async Task<IActionResult> PostRating(RatingNew rating) {
            var item = _mapper.Map<Rating>(rating);
            await _database.Ratings.AddAsync(item);
            await _database.SaveChangesAsync();
            return new CreatedResult("api/rating/" + item.ID, rating);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> PutData(RatingUpdate rating, int id) {
            //test if item exists
            var item = await _database.Ratings.FindAsync(id);
            if (item == null) {
                return new NotFoundResult();
            }

            _database.Ratings.Update(item);
            item.Update(_mapper.Map<Rating>(rating));
            await _database.SaveChangesAsync();
            return new JsonResult(item);
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteData(int id) {
            //test if item exists
            var item = await _database.Ratings.FindAsync(id);
            if (item == null) {
                return new NotFoundResult();
            }

            _database.Ratings.Remove(item);
            await _database.SaveChangesAsync();
            return new OkResult();
        }
    }
}